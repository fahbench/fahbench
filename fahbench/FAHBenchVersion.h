#ifndef FAHBENCH_VERSION_H_
#define FAHBENCH_VERSION_H_

#include <string>
#include <sstream>
#include <OpenMM.h>

#define FAHBench_VERSION_MAJOR @FAHBench_VERSION_MAJOR@
#define FAHBench_VERSION_MINOR @FAHBench_VERSION_MINOR@
#define FAHBench_VERSION_REVIS @FAHBench_VERSION_REVIS@

#define FAHBench_VERSION_REFSPEC "@FAHBench_VERSION_REFSPEC@"
#define FAHBench_VERSION_SHA "@FAHBench_VERSION_SHA@"

#cmakedefine FAHBench_VERSION_ISRELEASE

inline std::string getVersion() {
    std::stringstream ss;
    ss << FAHBench_VERSION_MAJOR << "." << FAHBench_VERSION_MINOR << "." << FAHBench_VERSION_REVIS;
    #ifndef FAHBench_VERSION_ISRELEASE
    std::string sha = FAHBench_VERSION_SHA;
    sha.resize(8);
    ss << "-dev-" << sha;
    #endif
    return ss.str();
}

#endif
