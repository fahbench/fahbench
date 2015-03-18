#include "gpuinfo.h"

#include <CL/cl.h>
#include <cuda.h>

#include <map>
#include <string>
#include <iostream>

#ifdef _WIN32
#include <windows.h>
#else
#include <dlfcn.h>
#include <dirent.h>
#include <cstdlib>
#endif

using namespace std;

// clears texts like "      intel xxxx" into
// "intel xxxx"
string formatRawChar(char *badForm, int size) {
    string foo;
    bool found = false;
    for(int i=0; i<size; i++) {
        if(found == false) {
            if(badForm[i] == ' ') {
                i++;
            } else {
                found = true;
                foo.push_back(badForm[i]);
            }
        } else {
            foo.push_back(badForm[i]);
        }
    }
    return foo.c_str();
}

map<string, OpenCLId> GPUInfo::getOpenCLDevices() {
    cl_platform_id platforms[100];
    cl_uint platforms_n = 0;
    clGetPlatformIDs(100, platforms, &platforms_n);
    if (platforms_n == 0)
        throw(std::runtime_error("No OpenCL Compatible Devices Found"));
    map<string, OpenCLId> openCLdevices;
    for(int j=0; j<platforms_n; j++) {
        cl_device_id devices[100];
        cl_uint devices_n = 0;
        clGetDeviceIDs(platforms[j], CL_DEVICE_TYPE_ALL, 100, devices, &devices_n);
        for (int i=0; i<devices_n; i++) {
            char buffer[10240];
            clGetDeviceInfo(devices[i], CL_DEVICE_NAME, sizeof(buffer), buffer, NULL);
            string formattedString = formatRawChar(buffer, 100);
            OpenCLId id;
            id.deviceId=i;
            id.platformId=j;
            openCLdevices[formattedString]=id;
        }
    }
    return openCLdevices;
}

typedef CUresult (__stdcall *cDGCPtr)(int *count);
typedef CUresult (__stdcall *cInitPtr)(unsigned int *flags);
typedef CUresult (__stdcall *cDGPtr)(CUdevice *device, int ordinal);
typedef CUresult (__stdcall *cDGNPtr)(char *name, int len, CUdevice device);

map<string, int> GPUInfo::getCUDADevices() {

    // Make sure CUDA SDK and VS2010 cl.exe exists first!

    // Tell Windows not to bother the user with ugly error boxes.
    const UINT oldErrorMode = SetErrorMode(SEM_FAILCRITICALERRORS);
    HMODULE handle = LoadLibrary("nvcuda.dll");
    SetErrorMode(oldErrorMode); // Restore previous error mode.
    if (handle == NULL) {
        throw std::runtime_error("CUDA ERROR: cannot find or load nvcuda.dll");
    }

    CUresult errorMsg;

    cInitPtr cuInit = (cInitPtr) GetProcAddress(handle, "cuInit");
    cDGCPtr cuDeviceGetCount = (cDGCPtr) GetProcAddress(handle, "cuDeviceGetCount");
    cDGPtr cuDeviceGet = (cDGPtr) GetProcAddress(handle, "cuDeviceGet");
    cDGNPtr cuDeviceGetName = (cDGNPtr) GetProcAddress(handle, "cuDeviceGetName");

    if(cuInit == NULL)
        throw std::runtime_error("CUDA ERROR: cannot GetProcAddress cuInit");
    if(cuDeviceGetCount == NULL)
        throw std::runtime_error("CUDA ERROR: cannot GetProcAddress cuDeviceGetCount");
    if(cuDeviceGet == NULL)
        throw std::runtime_error("CUDA ERROR: cannot GetProcAddress cuDeviceGet");
    if(cuDeviceGetName == NULL)
        throw std::runtime_error("CUDA ERROR: cannot GetProcAddress cuDeviceGetName");

    errorMsg = (*cuInit)(0);
    if(errorMsg != CUDA_SUCCESS)
        throw std::runtime_error("CUDA ERROR: cannot initialize CUDA");

    int numDevices = 0;
    errorMsg = (*cuDeviceGetCount)(&numDevices);
    if(errorMsg != CUDA_SUCCESS)
        throw std::runtime_error("CUDA ERROR: cannot get number of devices.");

    map<string, int> deviceMap;
    for(int i=0; i<numDevices; i++) {
        CUdevice device;
        cuDeviceGet(&device, i);
        char name[500];
        cuDeviceGetName(name, 500, device);
        deviceMap[name] = i;
    }
    return deviceMap;
}
