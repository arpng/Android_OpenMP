//
// Created by aris on 20-Jun-25.
//

#include "ocl_fibonacci.h"

// OpenCL kernel
const char* programSourceFibonacci =

        "__kernel void fibonacci(__global unsigned int* output, const unsigned int n) {"
        "   int i = get_global_id(0);"
        "   if (i >= n) return;"

        "   if (i == 0) {"
        "       output[i] = 0;"
        "   } else if (i == 1) {"
        "       output[i] = 1;"
        "   } else {"
        "       unsigned int a = 0, b = 1, temp;"
        "       for (int j = 2; j <= i; ++j) {"
        "       temp = a + b;"
        "       a = b;"
        "       b = temp;"
        "       }"
        "   output[i] = b;"
        "   }"
        "}";

JNIEXPORT jstring JNICALL
Java_com_example_idklol_MainActivity_stringFromJNIOclFibonacci(JNIEnv *env, jobject thiz) {
    // Allocate memory for vectors A, B, and C on the host
    unsigned int n = 50;
    int *result_h = (int*) malloc(sizeof(int) * n);

    char buffer[512];
    char temp[256];

    // Get platform and device information
    cl_platform_id platform;
    cl_device_id device;
    clGetPlatformIDs(1, &platform, NULL);
    clGetDeviceIDs(platform, CL_DEVICE_TYPE_GPU, 1, &device, NULL);

    // Create an OpenCL context and command queue
    cl_context context = clCreateContext(NULL, 1, &device, NULL, NULL, NULL);
    cl_command_queue queue = clCreateCommandQueue(context, device, 0, NULL);

    // Create memory buffers on the device
    cl_mem result_d = clCreateBuffer(context, CL_MEM_READ_ONLY, n * sizeof(int), NULL, NULL);

    // Copy vectors A and B to the device
    clEnqueueWriteBuffer(queue, result_d, CL_TRUE, 0, n * sizeof(int), result_h, 0, NULL, NULL);

    // Create a program from the kernel source
    cl_program program = clCreateProgramWithSource(context, 1, &programSourceFibonacci, NULL, NULL);
    clBuildProgram(program, 1, &device, NULL, NULL, NULL);

    // Create the OpenCL kernel
    cl_kernel kernel = clCreateKernel(program, "fibonacci", NULL);

    // Set the arguments for the kernel
    clSetKernelArg(kernel, 0, sizeof(cl_mem), &result_d);
    clSetKernelArg(kernel, 1, sizeof(int), &n);

    // Execute the kernel
    size_t globalSize = n;
    size_t localSize = 1;

    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);  // Start time

    clEnqueueNDRangeKernel(queue, kernel, 1, NULL, &globalSize, &localSize, 0, NULL, NULL);

    clock_gettime(CLOCK_MONOTONIC, &end);  // End time

    // Read the result vector C back to the host
    clEnqueueReadBuffer(queue, result_d, CL_TRUE, 0, n * sizeof(int), result_h, 0, NULL, NULL);

    // Display the result
    for (int i = 0; i < n; i++) {
        sprintf(temp, "%d ", result_h[i]);
        strcat(buffer, temp);  // Concatenate temp to big_buffer
    }

    sprintf(temp, "\nlocalSize = %zu [work-items(threads) per work group] (time = %f)\n", localSize, (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9);
    strcat(buffer, temp);  // Concatenate temp to big_buffer

    // Clean up
    clReleaseMemObject(result_d);
    clReleaseProgram(program);
    clReleaseKernel(kernel);
    clReleaseCommandQueue(queue);
    clReleaseContext(context);

    free(result_h);

    return (*env)->NewStringUTF(env, buffer);
}
