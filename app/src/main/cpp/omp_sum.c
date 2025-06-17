//
// Created by aris on 20-Sep-24.
//

#include "omp_sum.h"

JNIEXPORT jstring JNICALL
Java_com_example_idklol_MainActivity_stringFromJNIOmpSum(JNIEnv *env, jobject thiz){

    int i;
    double sum_serial = 0.0;
    double sum_parallel = 0.0;
    char buffer[250];
    double start1, start2, end1, end2;

    // Serial execution
    start1 = omp_get_wtime();
    for (i = 0; i < N; i++) {
        sum_serial += 1.0 / (i + 1);
    }
    end1 = omp_get_wtime();

    // Parallel execution with OpenMP
    start2 = omp_get_wtime();
    #pragma omp parallel for reduction(+:sum_parallel) num_threads(4) schedule(static)
    for (i = 0; i < N; i++) {
        sum_parallel += 1.0 / (i + 1);
    }
    end2 = omp_get_wtime();

    sprintf(buffer, "Serial and parallel number calculations:\n\n"
                "[serial] sum = %f (time = %f)\n"
                "[openmp] sum = %f (time = %f)\n",
        sum_serial, (end1 - start1), sum_parallel, (end2 - start2));

    return (*env)->NewStringUTF(env, buffer);
}