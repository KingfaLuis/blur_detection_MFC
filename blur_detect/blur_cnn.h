#pragma once
#ifndef __BLUR_CNN_H__
#define __BLUR_CNN_H__


#include "blur.h"

BLUR_HANDLE blurcnn_init(std::string modelDir);
int blurcnn_uninit(BLUR_HANDLE handle);
int blurcnn_process_mat(BLUR_HANDLE handle, cv::Mat frame, float* out_put);


#endif//__SEATBELT_H__