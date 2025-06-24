//
// Created by aris on 19-Jun-25.
//

#include "ocl_pi.h"



JNIEXPORT jstring JNICALL
Java_com_example_idklol_MainActivity_stringFromJNIOpenClPi(JNIEnv *env, jobject thiz) {
    float W = 1.0f / N;
    float pi = 0.0f;

    cl_platform_id platform;
    cl_device_id device;
    cl_context context;
    cl_command_queue queue;
    cl_program program;
    cl_kernel kernel;

    cl_mem result_buffer;
//    float* h_results = malloc(sizeof(float) * N);

    const char* source =
            "__kernel void pi_kernel(__global float pi_d, const float W) {"
            "    int i = get_global_id(0);"
            "    float x = (i + 0.5) * W;"
            "    pi += 4.0 * W / (1.0 + x * x);"
            "}";

    // Get platform/device/context
    clGetPlatformIDs(1, &platform, NULL);
    clGetDeviceIDs(platform, CL_DEVICE_TYPE_GPU, 1, &device, NULL);
    context = clCreateContext(NULL, 1, &device, NULL, NULL, NULL);
    queue = clCreateCommandQueue(context, device, 0, NULL);

    // Create program and kernel
    program = clCreateProgramWithSource(context, 1, &source, NULL, NULL);
    clBuildProgram(program, 1, &device, NULL, NULL, NULL);
    kernel = clCreateKernel(program, "pi_kernel", NULL);

    // Allocate buffer
    result_buffer = clCreateBuffer(context, CL_MEM_WRITE_ONLY, sizeof(float), NULL, NULL);

    // Set kernel args
    clSetKernelArg(kernel, 0, sizeof(cl_mem), &result_buffer);
    clSetKernelArg(kernel, 1, sizeof(float), &W);

    size_t global_size = N;

    // Measure time
    clock_t start = clock();
    clEnqueueNDRangeKernel(queue, kernel, 1, NULL, &global_size, NULL, 0, NULL, NULL);
    clFinish(queue);
    clock_t end = clock();

    // Read back
    clEnqueueReadBuffer(queue, result_buffer, CL_TRUE, 0, sizeof(float), &pi, 0, NULL, NULL);

    // Sum on host
//    for (int i = 0; i < N; ++i)
//        pi += h_results[i];

    double elapsed = (double)(end - start) / CLOCKS_PER_SEC;

    // Format result
    char buffer[256];
    snprintf(buffer, sizeof(buffer), "[OpenCL] pi = %.10f (time = %.4f sec)", pi, elapsed);

    // Cleanup
//    free(h_results);
    clReleaseMemObject(result_buffer);
    clReleaseKernel(kernel);
    clReleaseProgram(program);
    clReleaseCommandQueue(queue);
    clReleaseContext(context);

    return (*env)->NewStringUTF(env, buffer);
}
