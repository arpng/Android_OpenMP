//
// Created by aris3 on 27-Sep-24.
//

#ifndef IDKLOL_OMPSUM_H
#define IDKLOL_OMPSUM_H

#include <jni.h>
#include <string.h>
#include <stdio.h>
#include <omp.h>

#define N 1000000000

JNIEXPORT jstring JNICALL Java_com_example_idklol_MainActivity_stringFromJNIC(JNIEnv *env, jobject thiz);

#endif //IDKLOL_OMPSUM_H
