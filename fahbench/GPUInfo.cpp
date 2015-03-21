#include "GPUInfo.h"

#include <CL/cl.h>
#include <cuda.h>
#include <cuda_runtime.h>

#include <map>
#include <string>
#include <iostream>

#ifdef _WIN32
#include <windows.h>
#endif

#define MAXDEVICES 100

using namespace std;

map<string, OpenCLId> GPUInfo::getOpenCLDevices() {
    cl_int error;

    // Get platforms
    cl_platform_id platforms[MAXDEVICES];
    cl_uint n_platforms = 0;
    error = clGetPlatformIDs(MAXDEVICES, platforms, &n_platforms);
    if (error)
        throw std::runtime_error("OpenCL Error: Cannot get platforms.");

    map<string, OpenCLId> openCLdevices;
    for(int j = 0; j < n_platforms; j++) {

        // Get devices for each platform
        cl_device_id devices[MAXDEVICES];
        cl_uint n_devices = 0;
        error = clGetDeviceIDs(platforms[j], CL_DEVICE_TYPE_ALL, MAXDEVICES, devices, &n_devices);
        if (error)
            throw std::runtime_error("OpenCL Error: Cannot get devices.");

        for (int i = 0; i < n_devices; i++) {
            char buffer[10240];
            error = clGetDeviceInfo(devices[i], CL_DEVICE_NAME, sizeof(buffer), buffer, NULL);
            if (error)
                throw std::runtime_error("OpenCL Error: Cannot get device info.");
            string device_name = buffer;

            // Make struct
            OpenCLId id;
            id.deviceId=i;
            id.platformId=j;
            openCLdevices[device_name] = id;
        }
    }
    return openCLdevices;
}

map<string, int> GPUInfo::getCUDADevices() {
    cudaError_t cu_error;

    int num_devices = 0;
    cu_error = cudaGetDeviceCount(&num_devices);
    if(cu_error != cudaSuccess)
        throw std::runtime_error("CUDA ERROR: cannot get number of devices.");

    map<string, int> cuda_devices;
    for(int i = 0; i < num_devices; i++) {
        cudaDeviceProp prop;
        cu_error = cudaGetDeviceProperties(&prop, i);
        if (cu_error != cudaSuccess)
            throw std::runtime_error("CUDA ERROR: Cannot get device properties.");
        cuda_devices[prop.name] = i;
    }
    return cuda_devices;
}