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

static string doubleToString(double value) {
    stringstream ss;
    ss << value;
    return ss.str();
};

static bool fexists(const char *filename) {
    ifstream ifile(filename);
    return ifile;
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

double SimulationWorker::benchmark(Context &context, int numSteps) {
    double stepSize = context.getIntegrator().getStepSize();
    clock_t startClock = clock();
    const int interval = 50;
    // This step call ensures everything has been JIT compiled
    context.getIntegrator().step(1);
    for(int i=0; i<numSteps; i++) {
        stringstream ss;
        ss.precision(4);
        ss << "benchmarking....";
        if(i % max((numSteps/interval),1) == 0 && i > 0) {
            double estimateClock = clock();
            double estimateTimeInSec = (double) (estimateClock-startClock) / (double) CLOCKS_PER_SEC;
            double estimateNsPerDay = (86400.0 / estimateTimeInSec)	* i * stepSize / 1000.0;

            ss << doubleToString(double(i)/double(numSteps)*100) << "% done,"
               << " estimate: ~" << estimateNsPerDay << " ns/day";
            updateProgress(ss.str());
        }
        if(i % 50000 == 0)
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
      cout << "Loading plugins from " << Platform::getDefaultPluginsDirectory() << std::endl;
      Platform::loadPluginsFromDirectory(Platform::getDefaultPluginsDirectory());
      cout << "Number of registered plugins " << Platform::getNumPlatforms() << std::endl;
      Platform & platform = Platform::getPlatformByName(simulation.platform);
      /*
        char buffer[10000];
        getcwd(buffer, sizeof(buffer));
        string s_cwd = buffer;
        if(simulation.platform.compare("OpenCL") == 0) {
            if(OpenCLLoaded_ == false) {
                Platform::loadPluginLibrary((s_cwd+"/OpenMMOpenCL.dll").c_str());
                OpenCLLoaded_ = true;
            }
        } else if(simulation.platform.compare("CUDA") == 0) {
            if(CUDALoaded_ == false) {
                Platform::loadPluginLibrary((s_cwd+"/OpenMMCUDA.dll").c_str());
                CUDALoaded_ = true;
            }
        }
        */
        /*
        for(auto it = simulation.properties.begin(); it!=simulation.properties.end(); it++) {
            cout << it->first << " " << it->second << endl;
        }
        */
        stringstream logFile;
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
        double score = benchmark(context, simulation.numSteps);
        //double score = 0;
        // no good portable way of doing this for both CLI and GUI
        // hacky for now, ideally we don't pass QLineEdit pointers but connect any type of string object
        if(window_ != NULL) {
            updateProgress(doubleToString(score) + " ns/day");
        } else {
            cout << score << endl;
        }

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
