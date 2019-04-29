#include <stdio.h>
#include <algorithm>
#include <vector>
#include <opencv2/opencv.hpp>
#include <string>
#include <list>
#include <map>
#include "net.h"
#include "reblur.h"
#include "util.h"
#include "aw_log.h"
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include<iostream>
#include<string>
#include<fstream>
#include <io.h> 
#include <stdlib.h> 
#include<time.h>
using namespace std;


#define BLUR_MAGIC 0xface0010
typedef struct _blur_handle
{
	int magic;
} blur_handle_t;

BLUR_HANDLE reblur_init(std::string modelDir)
{
	int ret = -1;

	blur_handle_t *hdl = (blur_handle_t *)AW_MALLOC(sizeof(blur_handle_t));
	if (hdl == NULL)
	{
		aw_log_err("blur init failed (malloc %d failed)", sizeof(blur_handle_t));
		return NULL;
	}

	hdl->magic = BLUR_MAGIC;
	aw_log_debug("blur load model success\n");
	aw_log_info("blur init success\n");
	return hdl;
}

API_EXPORT int reblur_uninit(BLUR_HANDLE handle)
{
	blur_handle_t *blur = (blur_handle_t *)handle;
	if (blur == NULL || blur->magic != BLUR_MAGIC)
	{
		aw_log_err("blur uninit failed (hanlde is invalid)");
		return -1;
	}

	blur->magic = 0;
	AW_FREE(blur);
	aw_log_info("BLUR uninit success\n");
	return 0;
}


void calDifferenceHor(const cv::Mat & input, cv::Mat &output)
{
	output = input.clone();
	int rows = output.rows;
	int cols = output.cols;
	for (int col = 0; col < cols; col++)
		for (int row = 1; row < rows; row++)
		{
			output.at<float>(row, col) = abs(input.at<float>(row, col) - input.at<float>(row - 1, col));
		}
}

void calDifferenceVer(const cv::Mat & input, cv::Mat &output)
{
	output = input.clone();
	int rows = output.rows;
	int cols = output.cols;
	for (int row = 0; row < rows; row++)
		for (int col = 1; col < cols; col++)
		{
			output.at<float>(row, col) = abs(input.at<float>(row, col) - input.at<float>(row, col - 1));
		}
}

void calV(const cv::Mat &m1, const cv::Mat &m2, cv::Mat &output)
{
	output = m1.clone();
	int rows = output.rows;
	int cols = output.cols;
	for (int row = 1; row < rows; row++)
		for (int col = 1; col < cols; col++)
		{
			output.at<float>(row, col) = max(0.0f, m1.at<float>(row, col) - m2.at<float>(row, col));
		}
}

float sumofCoefficient(const cv::Mat &d_input)
{
	int rows = d_input.rows;
	int cols = d_input.cols;
	float ret = 0;
	for (int row = 1; row < rows; row++)
		for (int col = 1; col < cols; col++)
		{
			ret += d_input.at<float>(row, col);
		}
	return ret;
}

float Reblur(const cv::Mat &input)
{
	//读取照片，转化为_F
	cv::Mat gray;
	cv::Mat _F;
	cv::Mat _Bver;
	cv::Mat _Bhor;
	cv::cvtColor(input, gray, CV_RGB2GRAY);
	gray.convertTo(_F, CV_32F);
	//创建滤波器，进行滤波并赋给 _Bver，_Bhor
	float k[9] = {
		1.f / 9,1.f / 9,1.f / 9,
		1.f / 9,1.f / 9,1.f / 9,
		1.f / 9,1.f / 9,1.f / 9 };
	cv::Mat Km = cv::Mat(1, 9, CV_32F, k);
	cv::filter2D(_F, _Bver, _F.depth(), Km, cv::Point(-1, -1));
	cv::filter2D(_F, _Bhor, _F.depth(), Km.t(), cv::Point(-1, -1));
	//水平差分、竖直差分
	cv::Mat d_Bver, d_Bhor, d_Fver, d_Fhor;
	cv::Mat Vver, Vhor;
	calDifferenceVer(_F, d_Fver);
	calDifferenceHor(_F, d_Fhor);
	calDifferenceVer(_Bver, d_Bver);
	calDifferenceHor(_Bhor, d_Bhor);
	//原图与二次模糊图片作差
	calV(d_Fver, d_Bver, Vver);
	calV(d_Fhor, d_Bhor, Vhor);
	//像素求和
	float s_Fver = sumofCoefficient(d_Fver);
	float s_Fhor = sumofCoefficient(d_Fhor);
	float s_Vver = sumofCoefficient(Vver);
	float s_Vhor = sumofCoefficient(Vhor);
	//归一化
	float b_Fver = (s_Fver - s_Vver) / s_Fver;
	float b_Fhor = (s_Fhor - s_Vhor) / s_Fhor;
	return max(b_Fver, b_Fhor);
}


API_EXPORT int reblur_process_mat(BLUR_HANDLE handle, cv::Mat src, float* out_put)
{
	*out_put = Reblur(src);
	return 0;
}