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

using namespace std;
using namespace OpenMM;

using std::string;
using std::map;


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


#ifdef WIN32
#include <float.h>
#define isnan(x) _isnan(x)
#endif

SimulationWorker::~SimulationWorker() {
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

template<class T>
T* SimulationWorker::loadObject(const string& fname) const {
    ifstream f(fname);
    if(!f) {
        throw std::runtime_error(boost::str(boost::format("cannot open %1%") % fname));
    }
    istream & s = f;
    return XmlSerializer::deserialize<T>(s);
}

double SimulationWorker::benchmark(Context &context, int numSteps, int nan_check_freq) {
    double stepSize = context.getIntegrator().getStepSize();
    const int interval = 50;
    // This step call ensures everything has been JIT compiled
    context.getIntegrator().step(1);

    // Start clock after JIT-ing. This used to be *before* in version < 2
    clock_t startClock = clock();

    for(int i=0; i<numSteps; i++) {
        stringstream ss;
        if(i % max((numSteps/interval),1) == 0 && i > 0) {
            double estimateClock = clock();
            double estimateTimeInSec = (double) (estimateClock-startClock) / (double) CLOCKS_PER_SEC;
            double estimateNsPerDay = (86400.0 / estimateTimeInSec)	* i * stepSize / 1000.0;
            double estimatePercent = (1.0 * i / numSteps) * 100;

            ss << boost::format("Progress: %1$2d%%\testimate: %2$.2f ns/day") % estimatePercent % estimateNsPerDay;
            updateProgress(ss.str());
        }
        if(nan_check_freq > 0 && i % nan_check_freq == 0)
            StateTests::checkForNans(context.getState(State::Positions | State::Velocities | State::Forces));
        context.getIntegrator().step(1);
    }
    // last getState makes sure everything in the queue has been flushed.
    State finalState = context.getState(State::Positions | State::Velocities | State::Forces | State::Energy);
    clock_t endClock = clock();
    double timeInSec = (double) (endClock-startClock) / (double) CLOCKS_PER_SEC;
    double nsPerDay = (86400.0 / timeInSec)	* numSteps * stepSize / 1000.0;
    StateTests::checkForNans(finalState);
    StateTests::checkForDiscrepancies(finalState);
    return nsPerDay;
}

void SimulationWorker::startSimulation(const Simulation & simulation) {

    if(simulation.window != NULL) {
        window_ = simulation.window;
        connect(this, SIGNAL(emitProgress(QString)), window_, SLOT(setText(const QString &)));
    }
    try {      
        //cout << "Loading plugins from " << Platform::getDefaultPluginsDirectory() << std::endl;
		cout << "Loading plugins from bin/" << std::endl;
        //Platform::loadPluginsFromDirectory(Platform::getDefaultPluginsDirectory());
		Platform::loadPluginsFromDirectory("bin");
        cout << "Number of registered plugins " << Platform::getNumPlatforms() << std::endl;
        Platform & platform = Platform::getPlatformByName(simulation.platform);

        updateProgress("deserializing system...");
        System * sys = loadObject<System>(simulation.sysFile);
        updateProgress("deserializing state...");
        State * state = loadObject<State>(simulation.stateFile);

        updateProgress("deserializing integrator...");
        Integrator * intg = loadObject<Integrator>(simulation.integratorFile);
        updateProgress("creating context...");
        Context context = Context(*sys, *intg, platform, simulation.getPropertiesMap());
        context.setState(*state);

        if(simulation.verifyAccuracy) {
            updateProgress("checking for accuracy...");
            Integrator* refIntg = loadObject<Integrator>(simulation.integratorFile);
            updateProgress("creating reference context...");
            Context refContext = Context(*sys, *refIntg, Platform::getPlatformByName("Reference"));
            updateProgress("setting state...");
            refContext.setState(*state);
            updateProgress("comparing forces and energy...");
            StateTests::compareForcesAndEnergies(refContext.getState(State::Forces | State::Energy), context.getState(State::Forces | State::Energy));
        }
        delete state;

        updateProgress("benchmarking...");
        double score = benchmark(context, simulation.numSteps, simulation.nan_check_freq);
        stringstream ss;
        ss << boost::format("%1$.4f ns/day") % score;
        updateProgress(ss.str());

        if(simulation.window != NULL)
            disconnect(this, SIGNAL(emitProgress(QString)), window_, SLOT(setText(const QString &)));

        emit simulationComplete();
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
