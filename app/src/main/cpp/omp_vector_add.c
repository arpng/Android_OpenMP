//
// Created by aris on 18-Jun-25.
//
#include "omp_vector_add.h"

void initialize_vectors(int* A, int* B){
    for (int i = 0; i < VECTOR_SIZE; i++) {
        A[i] = i;
        B[i] = i * 2;
    }
}

void serial_add_vectors(int* A, int* B, int* C){
    for (int i = 0; i < VECTOR_SIZE; i++) {
        C[i] = A[i] + B[i];
    }
}

void parallel_add_vectors(int* A, int* B, int* C){
    #pragma omp parallel for num_threads(4) schedule(guided)
    for (int i = 0; i < VECTOR_SIZE; i++) {
        C[i] = A[i] + B[i];
    }
}


JNIEXPORT jstring JNICALL
Java_com_example_idklol_MainActivity_stringFromJNIOmpVectorAdd(JNIEnv *env, jobject thiz) {
    int *A = (int *) malloc(sizeof(int) * VECTOR_SIZE);
    int *B = (int *) malloc(sizeof(int) * VECTOR_SIZE);
    int *C = (int *) malloc(sizeof(int) * VECTOR_SIZE);
    double start1, end1, start2, end2;

    char buffer[4096] = {0};  // Larger buffer to avoid overflow
    char temp[128];

    // Initialize vectors A and B
    initialize_vectors(A,B);

    // Run serial vector addition
    start1 = omp_get_wtime();
    serial_add_vectors(A, B, C);
    end1 = omp_get_wtime();

    sprintf(buffer, "Serial and parallel vector addition\n\n[serial]    (time = %f)\n", (end1 - start1));
    for (int i = 0; i < 4; i++) {
        sprintf(temp, "%d + %d = %d\n", A[(VECTOR_SIZE/4)*i], B[(VECTOR_SIZE/4)*i], C[(VECTOR_SIZE/4)*i]);
        strcat(buffer, temp);
    }


    // Run vector addition in parallel
    start2 = omp_get_wtime();
    parallel_add_vectors(A, B, C);
    end2 = omp_get_wtime();

    sprintf(temp, "[openmp]\n    (time = %f)\n", (end2 - start2));
    strcat(buffer, temp);
    for (int i = 0; i < 4; i++) {
        sprintf(temp, "%d + %d = %d\n", A[(VECTOR_SIZE/4)*i], B[(VECTOR_SIZE/4)*i], C[(VECTOR_SIZE/4)*i]);
        strcat(buffer, temp);
    }

    free(A);
    free(B);
    free(C);

    return (*env)->NewStringUTF(env, buffer);
}
