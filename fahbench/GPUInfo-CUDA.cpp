// Taken from http://stackoverflow.com/a/12854285
// Modified to use cuda runtime

#include "GPUInfo.h"

#include <cuda.h>
#include <cuda_runtime.h>

#ifdef _WIN32
#include <Windows.h>
#else
#include <dlfcn.h>
#define HMODULE void *
#endif

using std::vector;
using std::string;

HMODULE loadCudaLibrary() {
#ifdef _WIN32
#if _WIN64
    return LoadLibraryA("cudart64_65.dll");
#else
    return LoadLibraryA("cudart32_65.dll");
#endif
#else
    return dlopen("libcudart.so", RTLD_NOW);
#endif
}

void (*getProcAddress(HMODULE lib, const char * name))(void) {
#ifdef _WIN32
    return (void (*)(void)) GetProcAddress(lib, name);
#else
    return (void (*)(void)) dlsym(lib, (const char *)name);
#endif
}

int freeLibrary(HMODULE lib) {
#ifdef _WIN32
    return FreeLibrary(lib);
#else
    return dlclose(lib);
#endif
}

typedef cudaError_t (*cudaGetDeviceCount_pt)(int * count);
typedef cudaError_t (*cudaGetDeviceProperties_pt)(cudaDeviceProp *, int i);

vector<Device> GPUInfo::getCUDADevices() {
    HMODULE cu_rt;
    cudaGetDeviceCount_pt my_cuGetDeviceCount;
    cudaGetDeviceProperties_pt my_cuGetDeviceProperties;

    if ((cu_rt = loadCudaLibrary()) == nullptr)
        throw std::runtime_error("I couldn't find the Cuda runtime. Disabling CUDA.");
    if ((my_cuGetDeviceCount = (cudaGetDeviceCount_pt) getProcAddress(cu_rt, "cudaGetDeviceCount")) == nullptr)
        throw std::runtime_error("CUDA ERROR: could not load cudaGetDeviceCount");
    if ((my_cuGetDeviceProperties = (cudaGetDeviceProperties_pt) getProcAddress(cu_rt, "cudaGetDeviceProperties")) == nullptr)
        throw std::runtime_error("CUDA ERROR: could not load cudaGetDeviceProperties");

    cudaError_t cu_error;
    int num_devices = 0;
    cu_error = my_cuGetDeviceCount(&num_devices);
    if (cu_error != cudaSuccess)
        throw std::runtime_error("CUDA ERROR: cannot get number of devices.");

    vector<Device> cuda_devices;
    for (int i = 0; i < num_devices; i++) {
        cudaDeviceProp prop;
        cu_error = my_cuGetDeviceProperties(&prop, i);
        if (cu_error != cudaSuccess)
            throw std::runtime_error("CUDA ERROR: Cannot get device properties.");

        cuda_devices.push_back(Device("CUDA", prop.name, i));
    }
    freeLibrary(cu_rt);
    return cuda_devices;
}
