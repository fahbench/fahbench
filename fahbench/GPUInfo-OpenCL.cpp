#include <stdexcept>
#include "GPUInfo.h"

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
    for (cl_uint j = 0; j < n_platforms; j++) {

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

        for (cl_uint i = 0; i < n_devices; i++) {
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
