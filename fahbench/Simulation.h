#ifndef SIMULATION_H_
#define SIMULATION_H_

#include <string>
#include <sstream>
#include <map>
#include <boost/format.hpp>

#include <OpenMM.h>
#include "Updater.h"

#define NUMSTEPSEXPLICIT 1.5e4;
#define NUMSTEPSIMPLICIT 1e5;

using std::string;
using std::map;

class Simulation {

public:
    Simulation();

    // same across all runs
    std::string platform;
    std::string precision;

    int deviceId;
    int platformId;

    bool verifyAccuracy;
    int nan_check_freq;
    int numSteps;

    string solvent;

    // Set these, but use get methods

    void setSysFile(const string & sysFile);
    void setIntFile(const string & intFile);
    void setStateFile(const string & stateFile);

    map<string, string> getPropertiesMap() const;
    string getPluginDir() const;
    string getSysFile() const;
    string getIntFile() const;
    string getStateFile() const;

    std::string summary() const;

    void run(Updater & update) const;

private:
    bool use_built_in() const;

    string sysFile;
    string intFile;
    string stateFile;

    string openmm_data_dir;
    string openmm_plugin_dir;

    template<class T>
    T* loadObject(const string & fname) const;

    double benchmark(OpenMM::Context & context, Updater & update) const;
};


#endif // SIMULATION_H_
