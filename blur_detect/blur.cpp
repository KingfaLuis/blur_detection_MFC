#ifdef _WINDOWS
#include <emmintrin.h>
#endif
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>

#include "util.h"
#include "net.h"
#include "aw_log.h"
#include "blur.h"
#include "blur_cnn.h"
#include "reblur.h"


#define FR_MAGIC 0xface0002


typedef void * (*init)(std::string modelDir);
typedef int(*uninit)(BLUR_HANDLE handle);
typedef int(*process)(BLUR_HANDLE handle, cv::Mat src, float* out_put);

typedef struct
{
	int magic;
	void *handle;
	init init;
	uninit uninit;
	process process;
}blur_handle_t;

API_EXPORT BLUR_HANDLE blur_init(blur_option_e option, std::string modelDir)
{
	blur_handle_t *blur = (blur_handle_t *)AW_MALLOC(sizeof(blur_handle_t));
	blur->magic = FR_MAGIC;//todo
	switch (option) {
	case BLUR_OPTION_CNN:
		blur->init = blurcnn_init;
		blur->uninit = blurcnn_uninit;
		blur->process = blurcnn_process_mat;
		blur->handle = blur->init(modelDir);
		break;
	case BLUR_OPTION_REBLUR:
		blur->init = reblur_init;
		blur->uninit = reblur_uninit;
		blur->process = reblur_process_mat;
		blur->handle = blur->init(modelDir);
		break;

	}
	return blur;
}

API_EXPORT int blur_uninit(BLUR_HANDLE handle)
{
	blur_handle_t *blur = (blur_handle_t *)handle;
	if (blur == NULL || blur->magic != FR_MAGIC)
	{
		aw_log_err("fr uninit failed (fr hanlde is invalid)");
		return -1;
	}
	blur->uninit(blur->handle);
	blur->magic = 0;
	AW_FREE(blur);
	return 0;
}


API_EXPORT int blur_process(BLUR_HANDLE handle, cv::Mat src, float* out_put) {
	blur_handle_t *blur = (blur_handle_t *)handle;
	if (blur == NULL || blur->magic != FR_MAGIC)
	{
		aw_log_err("fr uninit failed (fr hanlde is invalid)");
		return -1;
	}
	blur->process(blur->handle, src, out_put);
	return 0;
}