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

Device::Device(const string & platform, const string & device, int device_id, int platform_id) :
    _platform(platform), _device(device), _device_id(device_id), _platform_id(platform_id) {
}

const string & Device::device() const {
    return _device;
}

const string & Device::platform() const {
    return _platform;
}

int Device::platform_id() const {
    return _platform_id;
}

int Device::device_id() const {
    return _device_id;
}

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

        for (int i = 0; i < n_devices; i++) {
            char buffer[10240];
            error = clGetDeviceInfo(devices[i], CL_DEVICE_NAME, sizeof(buffer), buffer, NULL);
            if (error)
                throw std::runtime_error("OpenCL Error: Cannot get device info.");
            string device_name = buffer;

            openCLdevices.push_back(Device("OpenCL", device_name, i, j));
        }
    }
    return openCLdevices;
}

#ifdef USE_CUDA
#include <cuda.h>
#include <cuda_runtime.h>
vector<Device> GPUInfo::getCUDADevices() {
    cudaError_t cu_error;

    int num_devices = 0;
    cu_error = cudaGetDeviceCount(&num_devices);
    if (cu_error != cudaSuccess)
        throw std::runtime_error("CUDA ERROR: cannot get number of devices.");

    vector<Device> cuda_devices;
    for (int i = 0; i < num_devices; i++) {
        cudaDeviceProp prop;
        cu_error = cudaGetDeviceProperties(&prop, i);
        if (cu_error != cudaSuccess)
            throw std::runtime_error("CUDA ERROR: Cannot get device properties.");

        cuda_devices.push_back(Device("CUDA", prop.name, i));
    }
    return cuda_devices;
}
#else // USE_CUDA
vector<Device> GPUInfo::getCUDADevices() {
    vector<Device> cuda_devices;
    return cuda_devices;
}
#endif // USE_CUDA
