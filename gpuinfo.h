#ifndef GPUINFO_H_
#define GPUINFO_H_

#include <map>
#include <string>

struct OpenCLId {
    int platformId;
    int deviceId;
};

namespace GPUInfo {

std::map<std::string, OpenCLId> getOpenCLDevices();

std::map<std::string, int> getCUDADevices();

}

#endif