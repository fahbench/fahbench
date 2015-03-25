#include <iostream>
#include <sstream>
#include <fstream>
#include <string>

#include <stdexcept>
#include <boost/format.hpp>

#include <QObject>
#include <OpenMM.h>
#include <openmm/serialization/XmlSerializer.h>

#include "StateTests.h"
#include "SimulationWorker.h"
#include "Utils.h"

#ifdef WIN32
#include <float.h>
#define isnan(x) _isnan(x)
#endif

using namespace std;
using namespace OpenMM;

using std::string;
using std::map;

static boost::format data_fmt("%1%/dhfr.%2%.%3%.xml");

Simulation::Simulation() : window(nullptr)
{
    openmm_data_dir = getExecutableDir() + "/../share/openmm_data";
    openmm_plugin_dir = getExecutableDir() + "/../lib/plugins";
}


map< string, string > Simulation::getPropertiesMap() const
{
    map<string,string> properties;
    if(platform == "CUDA") {
        properties["CudaPrecision"]=precision;
        properties["CudaDeviceIndex"]=std::to_string(deviceId);
    } else if(platform == "OpenCL") {
        properties["OpenCLPrecision"]=precision;
        properties["OpenCLDeviceIndex"]=std::to_string(deviceId);
        properties["OpenCLPlatformIndex"]=std::to_string(platformId);
    }
    return properties;
}

void Simulation::setSysFile(const std::string& sysFile)
{
    this->sysFile = sysFile;
}

void Simulation::setIntFile(const std::string& intFile)
{
    this->intFile = intFile;
}

void Simulation::setStateFile(const std::string& stateFile)
{
    this->stateFile = stateFile;
}

inline bool Simulation::use_built_in() const
{
    return (sysFile == "" && intFile == "" && stateFile == "");
}

std::string Simulation::getSysFile() const
{
    if(use_built_in()) {
        return boost::str(data_fmt % openmm_data_dir % "system" % solvent);
    }
    return sysFile;
}

std::string Simulation::getIntFile() const
{
    if(use_built_in()) {
        return boost::str(data_fmt % openmm_data_dir % "integrator" % solvent);
    }
    return intFile;
}

std::string Simulation::getStateFile() const
{
    if(use_built_in()) {
        return boost::str(data_fmt % openmm_data_dir % "state" % solvent);
    }
    return stateFile;
}

std::string Simulation::getPluginDir() const
{
    return openmm_plugin_dir;

}

string Simulation::summary() const
{
    std::stringstream ss;
    ss << "OpenMM Simulation" << std::endl;
    ss << "-----------------" << std::endl << std::endl;

    ss << "Plugin directory: " << getPluginDir() << std::endl;
    ss << "System XML: " << getSysFile() << std::endl;
    ss << "Integrator XML: " << getIntFile() << std::endl;
    ss << "State XML: " << getStateFile() << std::endl;
    ss << "Steps: " << numSteps << std::endl;
    // TODO: more
    return ss.str();
}


void Simulation::run(Updater& update) const {
    string plugin_dir = getPluginDir();
    update.message(boost::format("Loading plugins from %1%") % plugin_dir);
    Platform::loadPluginsFromDirectory(plugin_dir);
    update.message(boost::format("Number of registered plugins: %1%") % Platform::getNumPlatforms());
    Platform & platform = Platform::getPlatformByName(this->platform);

    update.message("Deserializing system...");
    System * sys = loadObject<System>(getSysFile());
    update.message("deserializing state...");
    State * state = loadObject<State>(getStateFile());

    update.message("deserializing integrator...");
    Integrator * intg = loadObject<Integrator>(getIntFile());
    update.message("creating context...");
    Context context = Context(*sys, *intg, platform, getPropertiesMap());
    context.setState(*state);

    if(verifyAccuracy) {
        update.message("checking for accuracy...");
        Integrator* refIntg = loadObject<Integrator>(getIntFile());
        update.message("creating reference context...");
        Context refContext = Context(*sys, *refIntg, Platform::getPlatformByName("Reference"));
        update.message("setting state...");
        refContext.setState(*state);
        update.message("comparing forces and energy...");
        StateTests::compareForcesAndEnergies(refContext.getState(State::Forces | State::Energy), context.getState(State::Forces | State::Energy));
    }
    delete state;

    update.message("benchmarking...");
    double score = benchmark(context, update);
    update.message(boost::format("%1$.4f ns/day") % score);
}

double Simulation::benchmark(Context& context, Updater& update) const {
    double stepSize = context.getIntegrator().getStepSize();
    const int interval = 50;
    // This step call ensures everything has been JIT compiled
    context.getIntegrator().step(1);

    // Start clock after JIT-ing. This used to be *before* in version < 2
    clock_t startClock = clock();

    for(int i=0; i<numSteps; i++) {
        if(i % max((numSteps/interval),1) == 0 && i > 0) {
            double estimateClock = clock();
            double estimateTimeInSec = (double) (estimateClock-startClock) / (double) CLOCKS_PER_SEC;
            double estimateNsPerDay = (86400.0 / estimateTimeInSec)     * i * stepSize / 1000.0;
            double estimatePercent = (1.0 * i / numSteps) * 100;

            update.message(boost::format("Progress: %1$2d%%\testimate: %2$.2f ns/day") % estimatePercent % estimateNsPerDay);
            update.progress(i);
        }
        if(nan_check_freq > 0 && i % nan_check_freq == 0)
            StateTests::checkForNans(context.getState(State::Positions | State::Velocities | State::Forces));
        context.getIntegrator().step(1);
    }
    // last getState makes sure everything in the queue has been flushed.
    State finalState = context.getState(State::Positions | State::Velocities | State::Forces | State::Energy);
    clock_t endClock = clock();
    double timeInSec = (double) (endClock-startClock) / (double) CLOCKS_PER_SEC;
    double nsPerDay = (86400.0 / timeInSec) * numSteps * stepSize / 1000.0;
    StateTests::checkForNans(finalState);
    StateTests::checkForDiscrepancies(finalState);
    return nsPerDay;
}

template<class T>
T* Simulation::loadObject(const string& fname) const {
    ifstream f(fname);
    if(!f) {
        throw std::runtime_error(boost::str(boost::format("cannot open %1%") % fname));
    }
    istream & s = f;
    return XmlSerializer::deserialize<T>(s);
}


SimulationWorker::SimulationWorker(QObject *parent) :
    QObject(parent),
    OpenCLLoaded_(false),
    CUDALoaded_(false),
    window_(NULL) {
}

void SimulationWorker::updateProgress(const string &text) const {
    if(window_ != NULL) {
        emit emitProgress(QString(text.c_str()));
    } else {
        cout << text << endl;
    }
}

void SimulationWorker::startSimulation(const Simulation & simulation) {

    if(simulation.window != NULL) {
        window_ = simulation.window;
        connect(this, SIGNAL(emitProgress(QString)), window_, SLOT(setText(const QString &)));
    }
    try {
      simulation.run(*this);
    } catch(std::exception &e) {
        if(simulation.window != NULL) {
            updateProgress(e.what());
            disconnect(this, SIGNAL(emitProgress(QString)), window_, SLOT(setText(const QString &)));
            emit simulationComplete();
        } else {
            throw;
        }
    }
}

void SimulationWorker::progress(int i)
{
    // TODO
  std::cout << "Progress called with i = " << i << std::endl;
}

void SimulationWorker::message(std::string s)
{
    // TODO
  std::cout << s;

}

void SimulationWorker::message(boost::format)
{
  //TODO

}


