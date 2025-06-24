#include "ocl_info.h"

JNIEXPORT jstring JNICALL
Java_com_example_idklol_MainActivity_stringFromJNIOpenClInfo(JNIEnv *env, jobject thiz) {

    char buffer[512] = {0};
    char temp[64] = {0};
    cl_int err;

    // Get all platforms
    cl_uint num_platforms = 0;
    err = clGetPlatformIDs(0, NULL, &num_platforms);
    if (err != CL_SUCCESS || num_platforms == 0) {
        LOGE("No OpenCL platforms found!");
        sprintf(buffer, "No OpenCL platforms found!\n");
        return (*env)->NewStringUTF(env, buffer);
    }

    cl_platform_id *platforms = (cl_platform_id *)malloc(sizeof(cl_platform_id) * num_platforms);
    clGetPlatformIDs(num_platforms, platforms, NULL);
    cl_platform_id platform = platforms[0];

    // Get all devices on platform
    cl_uint num_devices = 0;
    err = clGetDeviceIDs(platform, CL_DEVICE_TYPE_ALL, 0, NULL, &num_devices);
    if (err != CL_SUCCESS || num_devices == 0) {
        LOGE("No OpenCL devices found!");
        sprintf(buffer, "No OpenCL devices found!\n");
        free(platforms);
        return (*env)->NewStringUTF(env, buffer);
    }

    cl_device_id *devices = (cl_device_id *)malloc(sizeof(cl_device_id) * num_devices);
    clGetDeviceIDs(platform, CL_DEVICE_TYPE_ALL, num_devices, devices, NULL);
    cl_device_id device = devices[0];

    // Device info
    char name[256], vendor[256], version[256];
    size_t max_work_item_sizes[3];
    size_t max_work_group_size;
    cl_uint compute_units;
    cl_ulong global_mem_size, local_mem_size;

    clGetDeviceInfo(device, CL_DEVICE_NAME, sizeof(name), name, NULL);
    clGetDeviceInfo(device, CL_DEVICE_VENDOR, sizeof(vendor), vendor, NULL);
    clGetDeviceInfo(device, CL_DEVICE_VERSION, sizeof(version), version, NULL);
    clGetDeviceInfo(device, CL_DEVICE_MAX_WORK_ITEM_SIZES, sizeof(max_work_item_sizes), max_work_item_sizes, NULL);
    clGetDeviceInfo(device, CL_DEVICE_MAX_WORK_GROUP_SIZE, sizeof(size_t), &max_work_group_size, NULL);
    clGetDeviceInfo(device, CL_DEVICE_MAX_COMPUTE_UNITS, sizeof(cl_uint), &compute_units, NULL);
    clGetDeviceInfo(device, CL_DEVICE_GLOBAL_MEM_SIZE, sizeof(cl_ulong), &global_mem_size, NULL);
    clGetDeviceInfo(device, CL_DEVICE_LOCAL_MEM_SIZE, sizeof(cl_ulong), &local_mem_size, NULL);

    // Log results
    LOGI("OpenCL Device Info:");
    LOGI("Name: %s", name);
    LOGI("Vendor: %s", vendor);
    LOGI("Version: %s", version);
    LOGI("Max size of work-items: (%zu, %zu, %zu)",
         max_work_item_sizes[0], max_work_item_sizes[1], max_work_item_sizes[2]);
    LOGI("Max size of work-groups: %zu", max_work_group_size);
    LOGI("Number of compute units: %u", compute_units);
    LOGI("Global memory size (bytes): %llu", (unsigned long long)global_mem_size);
    LOGI("Local memory size per compute unit (bytes): %llu",
         (unsigned long long)(local_mem_size / compute_units));

    sprintf(buffer, "OpenCL Device Info:\n------------------------\n\n");
    sprintf(temp, "Name: %s\n", name);
    strcat(buffer, temp);
    sprintf(temp, "Vendor: %s\n", vendor);
    strcat(buffer, temp);
    sprintf(temp, "Version: %s\n", version);
    strcat(buffer, temp);
    sprintf(temp, "Max size of work-items: (%zu, %zu, %zu)\n",
            max_work_item_sizes[0], max_work_item_sizes[1], max_work_item_sizes[2]);
    strcat(buffer, temp);
    sprintf(temp, "Max size of work-groups: %zu\n", max_work_group_size);
    strcat(buffer, temp);
    sprintf(temp, "Number of compute units: %u\n", compute_units);
    strcat(buffer, temp);
    sprintf(temp, "Global memory size (bytes): %llu\n", (unsigned long long)global_mem_size);
    strcat(buffer, temp);
    sprintf(temp, "Local memory size per compute unit (bytes): %llu\n",
            (unsigned long long)(local_mem_size / compute_units));
    strcat(buffer, temp);

    // Cleanup
    free(platforms);
    free(devices);
    return (*env)->NewStringUTF(env, buffer);
}
