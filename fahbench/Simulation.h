#ifndef SIMULATION_H_
#define SIMULATION_H_

#include <string>
#include <sstream>
#include <map>
#include <boost/format.hpp>

#include <OpenMM.h>

#include "DLLDefines.h"
#include "Updater.h"
#include "WorkUnit.h"



using std::string;
using std::map;

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
    string getPluginDir() const;

    std::string summary() const;

    double run(Updater & update) const;

private:

    WorkUnit _work_unit;

    string openmm_data_dir;
    string openmm_plugin_dir;

    template<class T>
    T * loadObject(const string & fname) const;

    double benchmark(OpenMM::Context & context, Updater & update) const;
};


#endif // SIMULATION_H_
