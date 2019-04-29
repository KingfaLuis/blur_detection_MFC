#pragma once
#ifndef __REBLUR_H__
#define __REBLUR_H__


#include "blur.h"
BLUR_HANDLE reblur_init(std::string modelDir);
int reblur_uninit(BLUR_HANDLE handle);
int reblur_process_mat(BLUR_HANDLE handle, cv::Mat frame, float* out_put);


#endif//__SEATBELT_H__#pragma once