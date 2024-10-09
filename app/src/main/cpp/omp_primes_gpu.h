//
// Created by aris3 on 09-Oct-24.
//

#ifndef IDKLOL_OMP_PRIMES_GPU_H
#define IDKLOL_OMP_PRIMES_GPU_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <time.h>
#include <math.h>
#include <sys/time.h>
#include <omp.h>
#include <jni.h>

#pragma pack(push, 2)
typedef struct bmpheader_
{
    char sign;
    int size;
    int notused;
    int data;
    int headwidth;
    int width;
    int height;
    short numofplanes;
    short bitpix;
    int method;
    int arraywidth;
    int horizresol;
    int vertresol;
    int colnum;
    int basecolnum;
} bmpheader_t;
#pragma pack(pop)

/* This is the image structure, containing all the BMP information
 * plus the RGB channels.
 */
typedef struct img_
{
    bmpheader_t header;
    int rgb_width;
    unsigned char *imgdata;
    unsigned char *red;
    unsigned char *green;
    unsigned char *blue;
} img_t;

void gaussian_blur_serial(int, img_t *, img_t *);
void gaussian_blur_omp_device(int radius, img_t *imgin, img_t *imgout);
JNIEXPORT jstring JNICALL
Java_com_example_idklol_MainActivity_stringFromJNICGPU(JNIEnv *env, jobject thiz);

#endif //IDKLOL_OMP_PRIMES_GPU_H
