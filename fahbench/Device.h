#ifndef DEVICE_H_
#define DEVICE_H_

#include "DLLDefines.h"
#include <string>

class FAHBENCH_EXPORT Device {
public:
    Device(const std::string & platform, const std::string & device, int device_id, int platform_id = 0);

    const std::string & device() const;
    int device_id() const;
    int platform_id() const;
    const std::string & platform() const;

    std::string platform_version;
    std::string device_version;

private:
    std::string _device;
    std::string _platform;
    int _device_id;
    int _platform_id;
};

#endif // DEVICE_H_