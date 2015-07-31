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

Simulation::Simulation()
    : work_unit(nullptr)
    , platform("OpenCL")
    , precision("single")
    , deviceId(0)
    , platformId(0)
    , verifyAccuracy(true)
    , nan_check_freq(0)
    , run_length(60)
    , openmm_plugin_dir(fs::canonical(getExecutableDir() / fs::path("../lib/plugins")))

{ }


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
    ss << "-------------------" << std::endl;

    ss << "Plugin directory: " << openmm_plugin_dir << std::endl;
    ss << "Work unit: " << work_unit->codename() << std::endl;
    ss << "WU Name: " << work_unit->fullname() << std::endl;
    ss << "WU Description: " << work_unit->description() << std::endl;
    ss << "System XML: " << work_unit->system_fn() << std::endl;
    ss << "Integrator XML: " << work_unit->integrator_fn() << std::endl;
    ss << "State XML: " << work_unit->state_fn() << std::endl;
    ss << "Step chunk: " << work_unit->step_chunk() << std::endl;
    ss << "Device ID " << deviceId << "; Platform " << platform;
    if (platform == "OpenCL") {
        ss << "; Platform ID " << platformId << std::endl;
    } else {
        ss << std::endl;
    }
    ss << "Run length: " << run_length.count() << "s" << std::endl;
    // TODO: more
    return ss.str();
}


SimulationResult Simulation::run(Updater & update) const {
    update.message(boost::format("Loading plugins from plugin directory"));
    Platform::loadPluginsFromDirectory(openmm_plugin_dir.string());
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
    float step_size_ns = context.getIntegrator().getStepSize() / 1000.0;
    int steps = 0;
    int step_chunk = work_unit->step_chunk();

    using day = std::chrono::duration<float, std::ratio<86400>>;
    using ms = std::chrono::milliseconds;
    auto run_length_ms = std::chrono::duration_cast<ms>(run_length);

    // This step call ensures everything has been JIT compiled
    context.getIntegrator().step(1);

    int last_nan_check = 0;
    std::chrono::steady_clock clock;
    auto clock_start = clock.now();

    while (true) {
        context.getIntegrator().step(step_chunk);
        steps += step_chunk;

        auto clock_now = clock.now();
        auto duration = clock_now - clock_start;

        if (duration > run_length) {
            break;
        }

        float per_day = 1.0 / std::chrono::duration_cast<day>(duration).count();
        float ns_day = steps * step_size_ns * per_day;
        update.progress(std::chrono::duration_cast<ms>(duration).count(),
                        run_length_ms.count(), ns_day);

        if (nan_check_freq > 0 && steps - last_nan_check >  nan_check_freq) {
            StateTests::checkForNans(context.getState(State::Positions | State::Velocities | State::Forces));
            last_nan_check = steps;
        }
    }
    // last getState makes sure everything in the queue has been flushed.
    State finalState = context.getState(State::Positions | State::Velocities | State::Forces | State::Energy);

    auto duration = clock.now() - clock_start;
    float per_day = 1.0 / std::chrono::duration_cast<day>(duration).count();
    float ns_day = steps * step_size_ns * per_day;

    StateTests::checkForNans(finalState);
    StateTests::checkForDiscrepancies(finalState);
    update.progress(run_length_ms.count(), run_length_ms.count(), ns_day);
    return ns_day;
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



