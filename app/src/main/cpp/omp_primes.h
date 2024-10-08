//
// Created by aris3 on 27-Sep-24.
//

#ifndef IDKLOL_OMPPRIMES_H
#define IDKLOL_OMPPRIMES_H

#include <jni.h>
#include <string.h>
#include <omp.h>

void serial_primes(long int n);
void openmp_primes(long int n);
JNIEXPORT jstring JNICALL Java_com_example_idklol_MainActivity_stringFromJNIC(JNIEnv *env, jobject thiz);

#endif //IDKLOL_OMPPRIMES_H
