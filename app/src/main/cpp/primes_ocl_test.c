//
// Created by aris on 13-May-25.
//

#include "primes_ocl_test.h"

JNIEXPORT jstring JNICALL
Java_com_example_idklol_MainActivity_stringFromJNIPrimesOclTest(JNIEnv *env, jobject thiz) {
    const long n = 10000000; // You can make this dynamic if needed
    const int batch_size = 50000;
    const int total_work_items = (n - 1) / 2;
    const int num_batches = (total_work_items + batch_size - 1) / batch_size;

    cl_platform_id platform;
    cl_device_id device;
    clGetPlatformIDs(1, &platform, NULL);
    clGetDeviceIDs(platform, CL_DEVICE_TYPE_GPU, 1, &device, NULL);

    cl_context context = clCreateContext(NULL, 1, &device, NULL, NULL, NULL);
    cl_command_queue queue = clCreateCommandQueue(context, device, 0, NULL);

    // Kernel source
    const char *programSourcePrimes =
            "__kernel void serial_primes_parallel(const long n,"
            " __global int* count, __global int* lastprime, const int offset) {"
            " int i = get_global_id(0) + offset;"
            " if (i >= (n - 1) / 2) return;"
            " long num = 2 * i + 3;"
            " long divisor = 1, quotient, remainder;"
            " do { divisor += 2;"
            " quotient = num / divisor;"
            " remainder = num % divisor;"
            " } while (remainder && divisor <= quotient);"
            " if (remainder || divisor == num) {"
            " atomic_inc(count);"
            " atomic_xchg(lastprime, (int)num);"
            " }"
            "}";

    cl_program program = clCreateProgramWithSource(context, 1, &programSourcePrimes, NULL, NULL);
    clBuildProgram(program, 1, &device, NULL, NULL, NULL);
    cl_kernel kernel = clCreateKernel(program, "serial_primes_parallel", NULL);

    // Allocate device memory for count and lastprime
    int initial_count = 1;
    int initial_lastprime = 2;
    cl_mem countBuffer = clCreateBuffer(context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR,
                                        sizeof(int), &initial_count, NULL);
    cl_mem lastprimeBuffer = clCreateBuffer(context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR,
                                            sizeof(int), &initial_lastprime, NULL);

    // Launch in batches
    for (int batch = 0; batch < num_batches; ++batch) {
        int offset = batch * batch_size;
        int current_batch = batch_size;
        if (offset + current_batch > total_work_items)
            current_batch = total_work_items - offset;

        clSetKernelArg(kernel, 0, sizeof(cl_long), &n);
        clSetKernelArg(kernel, 1, sizeof(cl_mem), &countBuffer);
        clSetKernelArg(kernel, 2, sizeof(cl_mem), &lastprimeBuffer);
        clSetKernelArg(kernel, 3, sizeof(cl_int), &offset);

        size_t globalSize = current_batch;
        clEnqueueNDRangeKernel(queue, kernel, 1, NULL, &globalSize, NULL, 0, NULL, NULL);
    }

    // Wait for kernel completion
    clFinish(queue);

    // Read results back to host
    int count, lastprime;
    clEnqueueReadBuffer(queue, countBuffer, CL_TRUE, 0, sizeof(int), &count, 0, NULL, NULL);
    clEnqueueReadBuffer(queue, lastprimeBuffer, CL_TRUE, 0, sizeof(int), &lastprime, 0, NULL, NULL);

    // Format result
    char buffer[100];
    snprintf(buffer, sizeof(buffer), "Count: %d\nLast Prime: %d\n", count, lastprime);

    // Cleanup
    clReleaseMemObject(countBuffer);
    clReleaseMemObject(lastprimeBuffer);
    clReleaseKernel(kernel);
    clReleaseProgram(program);
    clReleaseCommandQueue(queue);
    clReleaseContext(context);

    return (*env)->NewStringUTF(env, buffer);
}

















/*
const char* programSourcePrimes =
        "__kernel void serial_primes_global(const long n, __global int* count, __global int* last, int offset) {"
        "    int i = get_global_id(0) + offset;"
        "    long num = 2 * i + 3;"
        "    if (i >= (n - 1) / 2) return;"
        "    long divisor = 1, quotient, remainder;"
        "    do {"
        "        divisor += 2;"
        "        quotient = num / divisor;"
        "        remainder = num % divisor;"
        "    } while (remainder && divisor <= quotient);"
        "    if (remainder || divisor == num) {"
        "        atomic_inc(count);"
        "        atomic_xchg(last, (int)num);"
        "    }"
        "}";


JNIEXPORT jstring JNICALL
Java_com_example_idklol_MainActivity_stringFromJNIPrimesOclTest(JNIEnv *env, jobject thiz) {
    const long n = 10000000;
    const int work_items = (n - 1) / 2;

    char buffer[128] = {0};

    // Get platform and device info
    cl_platform_id platform;
    cl_device_id device;
    clGetPlatformIDs(1, &platform, NULL);
    clGetDeviceIDs(platform, CL_DEVICE_TYPE_GPU, 1, &device, NULL);

    // Print device name
    char device_name[64];
    clGetDeviceInfo(device, CL_DEVICE_NAME, sizeof(device_name), device_name, NULL);
    sprintf(buffer, "Device: %s\n", device_name);

    // Create context and queue
    cl_context context = clCreateContext(NULL, 1, &device, NULL, NULL, NULL);
    cl_command_queue queue = clCreateCommandQueue(context, device, 0, NULL);

    // Create result buffers for count and last prime
    cl_mem countBuf = clCreateBuffer(context, CL_MEM_READ_WRITE, sizeof(int), NULL, NULL);
    cl_mem lastBuf  = clCreateBuffer(context, CL_MEM_READ_WRITE, sizeof(int), NULL, NULL);

    // Initialize them to count=1 (for 2) and last=2
    int countInit = 1, lastInit = 2;
    clEnqueueWriteBuffer(queue, countBuf, CL_TRUE, 0, sizeof(int), &countInit, 0, NULL, NULL);
    clEnqueueWriteBuffer(queue, lastBuf, CL_TRUE, 0, sizeof(int), &lastInit, 0, NULL, NULL);

    // Build program
    cl_program program = clCreateProgramWithSource(context, 1, &programSourcePrimes, NULL, NULL);
    clBuildProgram(program, 1, &device, NULL, NULL, NULL);
    cl_kernel kernel = clCreateKernel(program, "serial_primes_global", NULL);

    // Set kernel arguments
    clSetKernelArg(kernel, 0, sizeof(long), &n);
    clSetKernelArg(kernel, 1, sizeof(cl_mem), &countBuf);
    clSetKernelArg(kernel, 2, sizeof(cl_mem), &lastBuf);

    // Run kernel
//    size_t globalSize = work_items;
//    clEnqueueNDRangeKernel(queue, kernel, 1, NULL, &globalSize, NULL, 0, NULL, NULL);

    const int BATCH_SIZE = 1;
    int total_batches = (work_items + BATCH_SIZE - 1) / BATCH_SIZE;

    for (int b = 0; b < total_batches; ++b) {
        int start = b * BATCH_SIZE;
        int size = BATCH_SIZE;
        if (start + size > work_items)
            size = work_items - start;

        clSetKernelArg(kernel, 3, sizeof(int), &start);  // pass the offset to kernel

        size_t globalSize = size;
        clEnqueueNDRangeKernel(queue, kernel, 1, NULL, &globalSize, NULL, 0, NULL, NULL);
    }


    // Read results
    int count, last;
    clEnqueueReadBuffer(queue, countBuf, CL_TRUE, 0, sizeof(int), &count, 0, NULL, NULL);
    clEnqueueReadBuffer(queue, lastBuf, CL_TRUE, 0, sizeof(int), &last, 0, NULL, NULL);

    char info[64];
    sprintf(info, "Count: %d\nLast prime: %d\n", count, last);
    strcat(buffer, info);

    // Cleanup
    clReleaseMemObject(countBuf);
    clReleaseMemObject(lastBuf);
    clReleaseKernel(kernel);
    clReleaseProgram(program);
    clReleaseCommandQueue(queue);
    clReleaseContext(context);

    return (*env)->NewStringUTF(env, buffer);
}
//////////////////////////////////////////////////////////////////////////////////////
 const char* programSourcePrimes =
        "__kernel void find_primes(const long n) {"
        "   int i = get_global_id(0);"
        "   long num = 2 * i + 3;"
        "   if (i >= (n - 1) / 2) return;"
        "   long divisor = 1;"
        "   long quotient, remainder;"
        "   do {"
        "       divisor += 2;"
        "       quotient = num / divisor;"
        "       remainder = num % divisor;"
        "   } while (remainder && divisor <= quotient);"
        "   if (remainder || divisor == num) {"
//        "       results[i] = (int)num;"
//        "   } else {"
//        "       results[i] = 0;"
        "        count++;"
        "        lastprime = num;"
        "   }"
        "}";

JNIEXPORT jstring JNICALL
Java_com_example_idklol_MainActivity_stringFromJNIPrimesOclTest(JNIEnv *env, jobject thiz) {
    const long n = 1000000; // you can change this to any upper bound for primes 10000000
    const int work_items = (n - 1) / 2;

//    int *results = (int*) calloc(work_items, sizeof(int)); // store primes or 0
    char buffer[250] = {0};
    char temp[32];

    // Get platform and device information
    cl_platform_id platform;
    cl_device_id device;
    clGetPlatformIDs(1, &platform, NULL);
    clGetDeviceIDs(platform, CL_DEVICE_TYPE_GPU, 1, &device, NULL);

    // Print device name
    char device_name[25];
    char device_name2[40];
    clGetDeviceInfo(device, CL_DEVICE_NAME, sizeof(device_name), device_name, NULL);
    __android_log_print(ANDROID_LOG_INFO, "ANDROID_LOG_TAG", "Device name: %s", device_name);
    sprintf(device_name2, "Device name: %s\n", device_name);
    strcat(buffer, device_name2);

    // Create context and command queue
    cl_context context = clCreateContext(NULL, 1, &device, NULL, NULL, NULL);
    cl_command_queue queue = clCreateCommandQueue(context, device, 0, NULL);

    // Create buffer for results
    cl_mem resultBuffer = clCreateBuffer(context, CL_MEM_WRITE_ONLY, work_items * sizeof(int), NULL, NULL);

    // Build kernel
    cl_program program = clCreateProgramWithSource(context, 1, &programSourcePrimes, NULL, NULL);
    clBuildProgram(program, 1, &device, NULL, NULL, NULL);
    cl_kernel kernel = clCreateKernel(program, "find_primes", NULL);

    // Set kernel arguments
    clSetKernelArg(kernel, 0, sizeof(cl_mem), &resultBuffer);
    clSetKernelArg(kernel, 1, sizeof(long), &n);

    // Run kernel
    size_t globalSize = work_items;
    clEnqueueNDRangeKernel(queue, kernel, 1, NULL, &globalSize, NULL, 0, NULL, NULL);

    // Read result buffer
    clEnqueueReadBuffer(queue, resultBuffer, CL_TRUE, 0, work_items * sizeof(int), results, 0, NULL, NULL);

    // Count and find last prime
    int count = 1; // 2 is prime
    int lastprime = 2;
//    sprintf(temp, "2\n");
//    strcat(buffer, temp);

    for (int i = 0; i < work_items; ++i) {
        if (results[i] != 0) {
            count++;
            lastprime = results[i];
//            sprintf(temp, "%d\n", results[i]);
//            strcat(buffer, temp);
        }
    }

    sprintf(temp, "Count: %d\nLast prime: %d\n", count, lastprime);
    strcat(buffer, temp);

    // Clean up
    clReleaseMemObject(resultBuffer);
    clReleaseKernel(kernel);
    clReleaseProgram(program);
    clReleaseCommandQueue(queue);
    clReleaseContext(context);
    free(results);

    return (*env)->NewStringUTF(env, buffer);
}
*/