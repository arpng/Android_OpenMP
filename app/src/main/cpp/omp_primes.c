//
// Created by aris on 20-Sep-24.
//

#include "omp_primes.h"

#define UPTO 10000000

long int count,      /* number of primes */
lastprime;  /* the last prime found */


void serial_primes(long int n) {
    long int i, num, divisor, quotient, remainder;

    if (n < 2) return;
    count = 1;                         /* 2 is the first prime */
    lastprime = 2;

    for (i = 0; i < (n-1)/2; ++i) {    /* For every odd number */
        num = 2*i + 3;

        divisor = 1;
        do
        {
            divisor += 2;                  /* Divide by the next odd */
            quotient  = num / divisor;
            remainder = num % divisor;
        } while (remainder && divisor <= quotient);  /* Don't go past sqrt */

        if (remainder || divisor == num) /* num is prime */
        {
            count++;
            lastprime = num;
        }
    }
}


void openmp_primes(long int n) {
    long int i, num, divisor, quotient, remainder;

    if (n < 2) return;
    count = 1;                         /* 2 is the first prime */
    lastprime = 2;

    /*
     * Parallelize the serial algorithm but you are NOT allowed to change it!
     * Don't add/remove/change global variables
    */


#pragma omp parallel for private(num, divisor, quotient, remainder) reduction(max: lastprime) reduction(+: count) num_threads(8) schedule(guided)
    for (i = 0; i < (n-1)/2; ++i) {    /* For every odd number */
        num = 2*i + 3;

        divisor = 1;
        do
        {
            divisor += 2;                  /* Divide by the next odd */
            quotient  = num / divisor;
            remainder = num % divisor;
        } while (remainder && divisor <= quotient);  /* Don't go past sqrt */


        if (remainder || divisor == num) /* num is prime */
        {
            count++;
            lastprime = num;
        }
    }
}


JNIEXPORT jstring JNICALL
Java_com_example_idklol_MainActivity_stringFromJNIC(JNIEnv *env, jobject thiz) {

    double start, finish, start1, finish1;
    char buffer[250];
    char temp[64];

    /* Time the following to compare performance
     */
    start = omp_get_wtime();
    serial_primes(UPTO);        /* time it */
    finish = omp_get_wtime();

    sprintf(buffer, "Serial and parallel prime number calculations:\n\n"
                    "[serial] count = %ld, last = %ld (time = %f)\n",
            count, lastprime, (finish - start));

    start1 = omp_get_wtime();
    openmp_primes(UPTO);        /* time it */
    finish1 = omp_get_wtime();

    sprintf(temp,"[openmp] count = %ld, last = %ld (time = %f)\n",
           count, lastprime, (finish1 - start1));

    strcat(buffer,temp);

    return (*env)->NewStringUTF(env, buffer);
}