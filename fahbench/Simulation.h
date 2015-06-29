#ifndef SIMULATION_H_
#define SIMULATION_H_

#include <string>
#include <sstream>
#include <map>
#include <boost/format.hpp>
#include <boost/filesystem.hpp>

#include <OpenMM.h>

#include "DLLDefines.h"
#include "Updater.h"
#include "WorkUnit.h"



using std::string;
using std::map;
namespace fs = boost::filesystem;

class FAHBENCH_EXPORT Simulation {

public:
    Simulation();

    std::string platform;
    std::string precision;

    int deviceId;
    int platformId;

    bool verifyAccuracy;
    int nan_check_freq;
    int numSteps;

    map<string, string> getPropertiesMap() const;

    std::string summary() const;

    double run(Updater & update) const;

private:

    WorkUnit _work_unit;

    fs::path openmm_data_dir;
    fs::path openmm_plugin_dir;

    template<class T>
    T * loadObject(const string & fname) const;

    double benchmark(OpenMM::Context & context, Updater & update) const;
};


#endif // SIMULATION_H_
