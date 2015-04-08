#ifndef GPUINFO_H_
#define GPUINFO_H_

#include "DLLDefines.h"
#include <vector>
#include <string>


class FAHBENCH_EXPORT Device {
public:
    Device(const std::string & platform, const std::string & device, int device_id, int platform_id = 0);

    const std::string & device() const;
    int device_id() const;
    int platform_id() const;
    const std::string & platform() const;

private:
    std::string _device;
    std::string _platform;
    int _device_id;
    int _platform_id;
};

class FAHBENCH_EXPORT GPUInfo {
public:
    static std::vector<Device> getOpenCLDevices();
    static std::vector<Device> getCUDADevices();
};

#endif
