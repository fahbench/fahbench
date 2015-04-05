#ifndef GPUINFO_H_
#define GPUINFO_H_

#include "DLLDefines.h"
#include <map>
#include <string>

struct FAHBENCH_EXPORT OpenCLId {
    int platformId;
    int deviceId;
};

class FAHBENCH_EXPORT GPUInfo {
public:
    static std::map<std::string, OpenCLId> getOpenCLDevices();
    static std::map<std::string, int> getCUDADevices();
};

#endif
