//
// Created by aris on 19-Jun-25.
//

#include "ocl_image.h"

const char* programSourceCode =
        "__kernel void rgba_to_grayscale(__global const uchar* input,"
        "__global uchar* output,"
        "int width,"
        "int height,"
        "int channels) {"
        "int i = get_global_id(0);"
        "int totalPixels = width * height;"

        "if (i >= totalPixels) return;"

        "int x = i % width;"
        "int y = i / width;"
        "int idx = i * channels;"

        "uchar r = input[idx];"
        "uchar g = input[idx + 1];"
        "uchar b = input[idx + 2];"

        "uchar gray = (uchar)(0.299f * r + 0.587f * g + 0.114f * b);"
        "output[y * width + x] = gray;"
        "}";



JNIEXPORT jstring JNICALL
Java_com_example_idklol_MainActivity_stringFromJNIOclImage(JNIEnv *env, jobject thiz, jstring jpath, jstring joutput) {
//    if (argc < 2) {
//        printf("Usage: %s <image_filename>\n", argv[0]);
//        return 1;
//    }

    char buffer[10];

    int width, height, channels;
    const char *path = (*env)->GetStringUTFChars(env, jpath, 0);
    const char *outputPath = (*env)->GetStringUTFChars(env, joutput, 0);
    unsigned char *h_input = stbi_load(path, &width, &height, &channels, 0);
    if (!h_input) {
//        printf("Failed to load image: %s\n", argv[1]);
        __android_log_print(ANDROID_LOG_INFO, "ANDROID_LOG_TAG", "Failed to load image: %s", path);
        return 0;
    }
//
//    if (channels < 3) {
//        printf("Image must have at least 3 channels (RGB).\n");
//        stbi_image_free(h_input);
//        return 1;
//    }

    size_t total_pixels = width * height;
    size_t input_size = total_pixels * channels;
    size_t output_size = total_pixels;

    unsigned char *h_output = (unsigned char *)malloc(output_size);

    // Set up OpenCL
    cl_platform_id platform_id;
    cl_device_id device_id;
    cl_uint ret_num_devices, ret_num_platforms;
    cl_int ret;

    ret = clGetPlatformIDs(1, &platform_id, &ret_num_platforms);
    ret = clGetDeviceIDs(platform_id, CL_DEVICE_TYPE_DEFAULT, 1, &device_id, &ret_num_devices);

    cl_context context = clCreateContext(NULL, 1, &device_id, NULL, NULL, &ret);
    cl_command_queue queue = clCreateCommandQueueWithProperties(context, device_id, 0, &ret);

    cl_program program = clCreateProgramWithSource(context, 1, &programSourceCode, NULL, &ret);
    ret = clBuildProgram(program, 1, &device_id, NULL, NULL, NULL);

//    if (ret != CL_SUCCESS) {
//        char log[2048];
//        clGetProgramBuildInfo(program, device_id, CL_PROGRAM_BUILD_LOG, sizeof(log), log, NULL);
//        printf("Build error:\n%s\n", log);
//        return 1;
//    }

    cl_kernel kernel = clCreateKernel(program, "rgba_to_grayscale", &ret);

    // Allocate device buffers
    cl_mem d_input = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, input_size, h_input, &ret);
    cl_mem d_output = clCreateBuffer(context, CL_MEM_WRITE_ONLY, output_size, NULL, &ret);

    // Set kernel arguments
    ret = clSetKernelArg(kernel, 0, sizeof(cl_mem), &d_input);
    ret |= clSetKernelArg(kernel, 1, sizeof(cl_mem), &d_output);
    ret |= clSetKernelArg(kernel, 2, sizeof(int), &width);
    ret |= clSetKernelArg(kernel, 3, sizeof(int), &height);
    ret |= clSetKernelArg(kernel, 4, sizeof(int), &channels);

    // Execute kernel
    size_t global_work_size = total_pixels;
    ret = clEnqueueNDRangeKernel(queue, kernel, 1, NULL, &global_work_size, NULL, 0, NULL, NULL);

    // Read back result
    ret = clEnqueueReadBuffer(queue, d_output, CL_TRUE, 0, output_size, h_output, 0, NULL, NULL);

    // Save output image
    stbi_write_png(outputPath, width, height, 1, h_output, width);

    sprintf(buffer, "OclDone");

    // Cleanup
    clReleaseMemObject(d_input);
    clReleaseMemObject(d_output);
    clReleaseKernel(kernel);
    clReleaseProgram(program);
    clReleaseCommandQueue(queue);
    clReleaseContext(context);

    free(h_output);
    stbi_image_free(h_input);
//    free(source_str);
    free(h_output);
    (*env)->ReleaseStringUTFChars(env, jpath, path);
    (*env)->ReleaseStringUTFChars(env, joutput, outputPath);

    return (*env)->NewStringUTF(env, buffer);
}
