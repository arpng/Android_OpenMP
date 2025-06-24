//
// Created by aris on 15-Oct-24.
//

#ifndef IDKLOL_OCL_VECTOR_ADD_H
#define IDKLOL_OCL_VECTOR_ADD_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <CL/cl.h>
#include <jni.h>
#include <android/log.h>
#include <time.h>

#define VECTOR_SIZE 100000000

JNIEXPORT jstring JNICALL
Java_com_example_idklol_MainActivity_stringFromJNIOclTest(JNIEnv *env, jobject thiz);

#endif //IDKLOL_OCL_VECTOR_ADD_H
