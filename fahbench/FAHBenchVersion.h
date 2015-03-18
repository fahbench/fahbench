#ifndef FAHBENCH_VERSION_H_
#define FAHBENCH_VERSION_H_

#include <string>
#include <sstream>

#define FAHBench_VERSION_MAJOR 1
#define FAHBench_VERSION_MINOR 2
#define FAHBench_VERSION_REVIS 1

inline std::string getVersion() {
    std::stringstream ss;
    ss << "v" << FAHBench_VERSION_MAJOR << "." << FAHBench_VERSION_MINOR << "." << FAHBench_VERSION_REVIS;
    return ss.str();
}

#endif
