#include <stdio.h>
#include <algorithm>
#include <vector>
#include <opencv2/opencv.hpp>
#include <string>
#include <list>
#include <map>
#include "net.h"
#include "blur_cnn.h"
#include "util.h"
#include "aw_log.h"

#include "blurnet_id.h"
#include "blurnet.h"

#define BLUR_MAGIC 0xface0002
typedef struct _blur_handle
{
	int magic;
	aw_mutex_t lock;
	ncnn::Net *net;
} blur_handle_t;

BLUR_HANDLE blurcnn_init(std::string modelDir)
{

	char path[256];
	int ret = -1;

	blur_handle_t *hdl = (blur_handle_t *)AW_MALLOC(sizeof(blur_handle_t));
	if (hdl == NULL)
	{
		aw_log_err("blur init failed (malloc %d failed)", sizeof(blur_handle_t));
		return NULL;
	}
	hdl->net = new ncnn::Net();

	if (hdl->net == NULL)
	{
		aw_log_err("blur init failed (new net failed)");
		goto EXIT_2rd;
	}

	ret = hdl->net->load_param(blurnet_param_bin);
	ret = hdl->net->load_model(blurnet_bin);

	aw_mutex_init(&hdl->lock);
	hdl->magic = BLUR_MAGIC;
	aw_log_debug("blur load model success\n");
	aw_log_info("blur init success\n");
	return hdl;

EXIT_1st:
	delete hdl->net;
EXIT_2rd:
	AW_FREE(hdl);
}

API_EXPORT int blurcnn_uninit(BLUR_HANDLE handle)
{
	blur_handle_t *blur = (blur_handle_t *)handle;
	if (blur == NULL || blur->magic != BLUR_MAGIC)
	{
		aw_log_err("blur uninit failed (hanlde is invalid)");
		return -1;
	}
	if (blur->net != NULL)
	{
		delete blur->net;
	}
	aw_mutex_uninit(&blur->lock);
	blur->magic = 0;
	AW_FREE(blur);
	aw_log_info("BLUR uninit success\n");
	return 0;
}

API_EXPORT int blurcnn_process_mat(BLUR_HANDLE handle, cv::Mat src, float* out_put)
{
	blur_handle_t *blur = (blur_handle_t *)handle;
	if (blur == NULL || blur->magic != BLUR_MAGIC)
	{
		aw_log_err("BLUR uninit failed (BLUR hanlde is invalid)");
		return -1;
	}
	cv::Mat frame = src.clone();
	cv::cvtColor(frame, frame, CV_BGR2RGB);
	ncnn::Extractor ex = blur->net->create_extractor();
	ex.set_light_mode(true);
	int w = frame.cols;
	int h = frame.rows;
	// subtract 0, norm to 0 ~ 1
	ncnn::Mat in = ncnn::Mat::from_pixels_resize(frame.data, ncnn::Mat::PIXEL_RGB, w, h, 135, 135);
	float means[3] = { 78.0f, 78.0f, 78.0f };
	float norms[3] = { 0.0078125f,0.0078125f,0.0078125f };
	in.substract_mean_normalize(means, norms);

	ex.input(blurnet_param_id::BLOB_data, in);

	ncnn::Mat out;
	TIME_STATISTICS_START();
	ex.extract(blurnet_param_id::BLOB_prob, out);
	TIME_STATISTICS_END("BLUR extract");
	*out_put = out[1];
	return 0;
}