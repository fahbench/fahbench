#include "Device.h"

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