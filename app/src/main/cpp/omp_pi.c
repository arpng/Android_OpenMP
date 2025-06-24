//
// Created by aris on 18-Jun-25.
//

#include "omp_pi.h"

void serial_pi(double* pi, double* W){
    for (int i=0; i < N; i++)
        *pi += 4 * *W / (1 + (i+0.5)*(i+0.5)* *W* *W);
}

void parallel_pi(double* pi, double* W){
    int i;
    double pi_value = *pi;
    #pragma omp parallel for private(i) reduction(+:pi_value) num_threads(4) schedule(static)
    for (i=0; i < N; i++) {
        pi_value += 4 * *W / (1 + (i + 0.5) * (i + 0.5) * *W * *W);
    }
    *pi = pi_value;
}


JNIEXPORT jstring JNICALL
Java_com_example_idklol_MainActivity_stringFromJNIOmpPi(JNIEnv *env, jobject thiz) {
    double pi = 0.0, W = 1.0/N;
    double start1, end1, start2, end2;

    char buffer[264] = {0};  // Larger buffer to avoid overflow
    char temp[64];

    // Run serial pi calculation
    start1 = omp_get_wtime();
    serial_pi(&pi, &W);
    end1 = omp_get_wtime();

    sprintf(buffer, "Serial and parallel pi calculation\n\n[serial] pi = %.10lf (time = %f)\n", pi, (end1 - start1));

    pi = 0.0, W = 1.0/N;

    // Run pi calculation in parallel
    start2 = omp_get_wtime();
    parallel_pi(&pi, &W);
    end2 = omp_get_wtime();

    sprintf(temp, "[openmp] pi = %.10lf (time = %f)\n", pi, (end2 - start2));
    strcat(buffer, temp);

    return (*env)->NewStringUTF(env, buffer);
}
