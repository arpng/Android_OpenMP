//
// Created by aris on 15-Oct-24.
//

#include "ocl_test.h"


// OpenCL kernel for vector addition
const char* programSource =
        "__kernel void vecadd(__global int* A, __global int* B, __global int* C) {"
        "   int id = get_global_id(0);"
        "   C[id] = A[id] + B[id];"
        "}";

JNIEXPORT jstring JNICALL
Java_com_example_idklol_MainActivity_stringFromJNIOclTest(JNIEnv *env, jobject thiz) {
    // Allocate memory for vectors A, B, and C on the host
    int *A = (int*) malloc(sizeof(int) * VECTOR_SIZE);
    int *B = (int*) malloc(sizeof(int) * VECTOR_SIZE);
    int *C = (int*) malloc(sizeof(int) * VECTOR_SIZE);

    char buffer[250];
    char temp[20];

    // Initialize vectors A and B
    for (int i = 0; i < VECTOR_SIZE; i++) {
        A[i] = i;
        B[i] = i * 2;
    }

    // Get platform and device information
    cl_platform_id platform;
    cl_device_id device;
    clGetPlatformIDs(1, &platform, NULL);
    clGetDeviceIDs(platform, CL_DEVICE_TYPE_GPU, 1, &device, NULL);

    // Print device name, should be qualcomm adreno
    char device_name[25];
    char device_name2[40];
    clGetDeviceInfo(device, CL_DEVICE_NAME, sizeof(device_name), device_name, NULL);
//    printf("Device name: %s\n", device_name);
    __android_log_print(ANDROID_LOG_INFO, "ANDROID_LOG_TAG", "Device name: %s\n", device_name);
    sprintf(device_name2, "Device name: %s\n", device_name);
    strcat(buffer, device_name2);

    // Create an OpenCL context and command queue
    cl_context context = clCreateContext(NULL, 1, &device, NULL, NULL, NULL);
    cl_command_queue queue = clCreateCommandQueue(context, device, 0, NULL);

    // Create memory buffers on the device
    cl_mem bufferA = clCreateBuffer(context, CL_MEM_READ_ONLY, VECTOR_SIZE * sizeof(int), NULL, NULL);
    cl_mem bufferB = clCreateBuffer(context, CL_MEM_READ_ONLY, VECTOR_SIZE * sizeof(int), NULL, NULL);
    cl_mem bufferC = clCreateBuffer(context, CL_MEM_WRITE_ONLY, VECTOR_SIZE * sizeof(int), NULL, NULL);

    // Copy vectors A and B to the device
    clEnqueueWriteBuffer(queue, bufferA, CL_TRUE, 0, VECTOR_SIZE * sizeof(int), A, 0, NULL, NULL);
    clEnqueueWriteBuffer(queue, bufferB, CL_TRUE, 0, VECTOR_SIZE * sizeof(int), B, 0, NULL, NULL);

    // Create a program from the kernel source
    cl_program program = clCreateProgramWithSource(context, 1, &programSource, NULL, NULL);
    clBuildProgram(program, 1, &device, NULL, NULL, NULL);

    // Create the OpenCL kernel
    cl_kernel kernel = clCreateKernel(program, "vecadd", NULL);

    // Set the arguments for the kernel
    clSetKernelArg(kernel, 0, sizeof(cl_mem), &bufferA);
    clSetKernelArg(kernel, 1, sizeof(cl_mem), &bufferB);
    clSetKernelArg(kernel, 2, sizeof(cl_mem), &bufferC);

    // Execute the kernel
    size_t globalSize = VECTOR_SIZE;
    clEnqueueNDRangeKernel(queue, kernel, 1, NULL, &globalSize, NULL, 0, NULL, NULL);

    // Read the result vector C back to the host
    clEnqueueReadBuffer(queue, bufferC, CL_TRUE, 0, VECTOR_SIZE * sizeof(int), C, 0, NULL, NULL);

    // Display the result
    for (int i = 0; i < VECTOR_SIZE; i++) {
        sprintf(temp, "%d + %d = %d\n", A[i], B[i], C[i]);
        strcat(buffer, temp);  // Concatenate temp to big_buffer
    }

    // Clean up
    clReleaseMemObject(bufferA);
    clReleaseMemObject(bufferB);
    clReleaseMemObject(bufferC);
    clReleaseProgram(program);
    clReleaseKernel(kernel);
    clReleaseCommandQueue(queue);
    clReleaseContext(context);

    free(A);
    free(B);
    free(C);

    return (*env)->NewStringUTF(env, buffer);
}
