#pragma once
#pragma once
#define BLUR_HANDLE void*


typedef enum
{
	BLUR_OPTION_CNN = 0,
	BLUR_OPTION_REBLUR,
	BlUR_OPTION_LAPLACIAN,
} blur_option_e;

BLUR_HANDLE blur_init(blur_option_e option, std::string modelDir);
int blur_uninit(BLUR_HANDLE fr);
int blur_process(BLUR_HANDLE handle, cv::Mat src, float* out_put);

