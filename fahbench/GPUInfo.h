#ifndef GPUINFO_H_
#define GPUINFO_H_

#include <map>
#include <string>

struct OpenCLId {
    int platformId;
    int deviceId;
};

class GPUInfo {
public:
    static std::map<std::string, OpenCLId> getOpenCLDevices();
    static std::map<std::string, int> getCUDADevices();
};

#endif
