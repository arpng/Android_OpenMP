//
// Created by aris on 08-Oct-24.
//

#ifndef IDKLOL_OMPRIMESUSINGLIB_H
#define IDKLOL_OMPRIMESUSINGLIB_H

#include <jni.h>
#include <string.h>
#include <stdio.h>
#include <omp_primes_lib.h>

#define UPTO 10000000

JNIEXPORT jstring JNICALL
Java_com_example_idklol_MainActivity_stringFromJNICLib(JNIEnv *env, jobject thiz);

#endif //IDKLOL_OMPRIMESUSINGLIB_H
