#ifndef GPUINFO_H_
#define GPUINFO_H_

#include "DLLDefines.h"
#include "Device.h"
#include <vector>
#include <string>


class FAHBENCH_EXPORT GPUInfo {
public:
    static std::vector<Device> getOpenCLDevices();
    static std::vector<Device> getCUDADevices();
};

#endif // GPUINFO_H_
