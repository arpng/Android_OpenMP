//
// Created by aris on 08-Oct-24.
//

#include "omp_primes_using_lib.h"

JNIEXPORT jstring JNICALL
Java_com_example_idklol_MainActivity_stringFromJNICLib(JNIEnv *env, jobject thiz) {

    long int count, count1,      /* number of primes */
    lastprime, lastprime1;       /* the last prime found */
    double start, finish, start1, finish1;
    char* buffer = (char*) malloc(sizeof(char) * 250);

    /* Time the following to compare performance
     */
    start = omp_get_wtime();
    serial_primes_statlib(UPTO, &count, &lastprime);        /* time it */
    finish = omp_get_wtime();

    start1 = omp_get_wtime();
    openmp_primes_statlib(UPTO, &count1, &lastprime1/*, buffer */);        /* time it */
    finish1 = omp_get_wtime();

    sprintf(buffer, "Serial and parallel prime number calculations using ext lib:\n\n"
                    "[serial] count = %ld, last = %ld (time = %f)\n"
                    "[openmp] count = %ld, last = %ld (time = %f)\n",
            count, lastprime, (finish - start), count1, lastprime1, (finish1 - start1));

    return (*env)->NewStringUTF(env, buffer);
}
