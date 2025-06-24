//
// Created by aris on 19-Jun-25.
//

#ifndef IDKLOL_OCL_INFO_H
#define IDKLOL_OCL_INFO_H

#include <CL/cl.h>
#include <stdlib.h>
#include <string.h>
#include <android/log.h>
#include <jni.h>

#define LOG_TAG "OpenCL_INFO"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

#endif //IDKLOL_OCL_INFO_H
