#include "FAHBenchVersion.h"
#include "GPUInfo.h"

#include <stdexcept>
#include <CL/cl.h>

#ifdef _WIN32
#include <windows.h>
#endif

#define MAXDEVICES 100

using std::vector;
using std::string;

vector<Device> GPUInfo::getOpenCLDevices() {
    cl_int error;

    // Get platforms
    cl_platform_id platforms[MAXDEVICES];
    cl_uint n_platforms = 0;
    error = clGetPlatformIDs(MAXDEVICES, platforms, &n_platforms);
    if (error)
        throw std::runtime_error("OpenCL Error: Cannot get platforms.");

    vector<Device> openCLdevices;
    for (int j = 0; j < n_platforms; j++) {

        // Get devices for each platform
        cl_device_id devices[MAXDEVICES];
        cl_uint n_devices = 0;
        error = clGetDeviceIDs(platforms[j], CL_DEVICE_TYPE_ALL, MAXDEVICES, devices, &n_devices);
        if (error)
            throw std::runtime_error("OpenCL Error: Cannot get devices.");

        char plat_version_buffer[10240];
        error = clGetPlatformInfo(platforms[j], CL_PLATFORM_VERSION, sizeof(plat_version_buffer), plat_version_buffer, NULL);
        if (error)
            throw std::runtime_error("OpenCL Error: Cannot get platform version.");
        string platform_version = plat_version_buffer;

        for (int i = 0; i < n_devices; i++) {
            char buffer[10240];
            error = clGetDeviceInfo(devices[i], CL_DEVICE_NAME, sizeof(buffer), buffer, NULL);
            if (error)
                throw std::runtime_error("OpenCL Error: Cannot get device name.");
            string device_name = buffer;

            error = clGetDeviceInfo(devices[i], CL_DEVICE_VERSION, sizeof(buffer), buffer, NULL);
            if (error)
                throw std::runtime_error("OpenCL Error: Cannot get device version.");
            string device_version = buffer;

            auto device = Device("OpenCL", device_name, i, j);
            device.platform_version = platform_version;
            device.device_version = device_version;

            openCLdevices.push_back(device);
        }
    }
    return openCLdevices;
}


#ifdef USE_CUDA
// Taken from http://stackoverflow.com/a/12854285
// Modified to use cuda runtime

#include <cuda.h>
#include <cuda_runtime.h>

#ifdef _WIN32
#include <Windows.h>
#else
#include <dlfcn.h>
#define HMODULE void *
#endif


HMODULE loadCudaLibrary() {
#ifdef _WIN32
#if _WIN64
    return LoadLibraryA("cudart64_65.dll");
#else
    return loadLibraryA("cudart32_65.dll");
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
    // TODO: custom exception

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

#else // USE_CUDA
vector<Device> GPUInfo::getCUDADevices() {
    vector<Device> cuda_devices;
    return cuda_devices;
}
#endif // USE_CUDA
