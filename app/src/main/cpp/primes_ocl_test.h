//
// Created by aris on 13-May-25.
//

#ifndef IDKLOL_PRIMES_OCL_TEST_H
#define IDKLOL_PRIMES_OCL_TEST_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <CL/cl.h>
#include <jni.h>
#include <android/log.h>

#define VECTOR_SIZE 5

JNIEXPORT jstring JNICALL
Java_com_example_idklol_MainActivity_stringFromJNIPrimesOclTest(JNIEnv *env, jobject thiz);

#endif //IDKLOL_PRIMES_OCL_TEST_H
