#include <sstream>
#include <fstream>
#include <string>

#include <stdexcept>
#include <boost/format.hpp>

#include <OpenMM.h>
#include <openmm/serialization/XmlSerializer.h>

#include "StateTests.h"
#include "Simulation.h"
#include "Utils.h"

#ifdef WIN32
#include <float.h>
#define isnan(x) _isnan(x)
#endif

using namespace OpenMM;

using std::string;
using std::map;


static boost::format data_fmt("%1%/dhfr.%2%.%3%.xml");

Simulation::Simulation() {
    openmm_plugin_dir = fs::canonical(getExecutableDir() / fs::path("../lib/plugins"));
    work_unit = nullptr;
}


map< string, string > Simulation::getPropertiesMap() const {
    map<string, string> properties;
    if (platform == "CUDA") {
        properties["CudaPrecision"] = precision;
        properties["CudaDeviceIndex"] = std::to_string(deviceId);
    } else if (platform == "OpenCL") {
        properties["OpenCLPrecision"] = precision;
        properties["OpenCLDeviceIndex"] = std::to_string(deviceId);
        properties["OpenCLPlatformIndex"] = std::to_string(platformId);
    }
    return properties;
}

string Simulation::summary() const {
    std::stringstream ss;
    ss << "FAHBench Simulation" << std::endl;
    ss << "-----------------" << std::endl;

    ss << "Plugin directory: " << openmm_plugin_dir << std::endl;
    ss << "Work unit: " << work_unit->codename() << std::endl;
    ss << "WU Name: " << work_unit->fullname() << std::endl;
    ss << "WU Description: " << work_unit->description() << std::endl;
    ss << "System XML: " << work_unit->system_fn() << std::endl;
    ss << "Integrator XML: " << work_unit->integrator_fn() << std::endl;
    ss << "State XML: " << work_unit->state_fn() << std::endl;
    ss << "Steps: " << work_unit->n_steps();
    if (work_unit->user_n_steps()) {
        ss << " (user specified)" << std::endl;
    } else {
        ss << std::endl;
    }
    // TODO: more
    return ss.str();
}


SimulationResult Simulation::run(Updater & update) const {
    update.message(boost::format("Loading plugins from plugin directory"));
    Platform::loadPluginsFromDirectory(openmm_plugin_dir.native());
    update.message(boost::format("Number of registered plugins: %1%") % Platform::getNumPlatforms());
    Platform & platform = Platform::getPlatformByName(this->platform);

    update.message("Deserializing system...");
    System * sys = loadObject<System>(work_unit->system_fn());
    update.message("Deserializing state...");
    State * state = loadObject<State>(work_unit->state_fn());
    update.message("Deserializing integrator...");
    Integrator * intg = loadObject<Integrator>(work_unit->integrator_fn());

    update.message("Creating context...");
    Context context(*sys, *intg, platform, getPropertiesMap());
    context.setState(*state);

    if (verifyAccuracy) {
        update.message("Checking for accuracy...");
        Integrator * refIntg = loadObject<Integrator>(work_unit->integrator_fn());
        update.message("Creating reference context...");
        Context refContext(*sys, *refIntg, Platform::getPlatformByName("Reference"));
        refContext.setState(*state);
        update.message("Comparing forces and energy...");
        StateTests::compareForcesAndEnergies(refContext.getState(State::Forces | State::Energy), context.getState(State::Forces | State::Energy));
        delete refIntg;
    }

    update.message("Starting Benchmark");
    float score = benchmark(context, update);
    update.message("Benchmarking finished.");

    SimulationResult result(score, sys->getNumParticles());

    delete sys;
    delete state;
    delete intg;

    return result;
}

float Simulation::benchmark(Context & context, Updater & update) const {
    double stepSize = context.getIntegrator().getStepSize();
    const int rep_interval = 50; // TODO: configurable
    // This step call ensures everything has been JIT compiled
    context.getIntegrator().step(1);

    // Start clock after JIT-ing. This used to be *before* in version < 2
    clock_t startClock = clock();

    for (int i = 0; i < work_unit->n_steps(); i++) {
        if (i > 0 && i % rep_interval == 0) {
            double estimateClock = clock();
            double estimateTimeInSec = (double)(estimateClock - startClock) / (double) CLOCKS_PER_SEC;
            double estimateNsPerDay = (86400.0 / estimateTimeInSec) * i * stepSize / 1000.0;
            update.progress(i, work_unit->n_steps(), estimateNsPerDay);
        }
        if (nan_check_freq > 0 && i % nan_check_freq == 0)
            StateTests::checkForNans(context.getState(State::Positions | State::Velocities | State::Forces));
        context.getIntegrator().step(1);
    }
    // last getState makes sure everything in the queue has been flushed.
    State finalState = context.getState(State::Positions | State::Velocities | State::Forces | State::Energy);
    clock_t endClock = clock();
    float timeInSec = (float)(endClock - startClock) / (float) CLOCKS_PER_SEC;
    float nsPerDay = (86400.0 / timeInSec) * work_unit->n_steps() * stepSize / 1000.0;
    StateTests::checkForNans(finalState);
    StateTests::checkForDiscrepancies(finalState);
    return nsPerDay;
}

template<class T>
T * Simulation::loadObject(const string & fname) const {
    std::ifstream f(fname);
    if (!f) {
        throw std::runtime_error(boost::str(boost::format("cannot open %1%") % fname));
    }
    std::istream & s = f;
    return XmlSerializer::deserialize<T>(s);
}

Simulation::~Simulation() {
    if (work_unit) delete work_unit;
}



