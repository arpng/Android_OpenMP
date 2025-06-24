//
// Created by aris on 20-Jun-25.
//

#ifndef IDKLOL_OCL_FIBONACCI_H
#define IDKLOL_OCL_FIBONACCI_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <CL/cl.h>
#include <jni.h>
#include <android/log.h>
#include <time.h>

JNIEXPORT jstring JNICALL
Java_com_example_idklol_MainActivity_stringFromJNIOclFibonacci(JNIEnv *env, jobject thiz);

#endif //IDKLOL_OCL_FIBONACCI_H
