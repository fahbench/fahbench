#ifndef SIMULATION_WORKER_H_
#define SIMULATION_WORKER_H_

#include <QObject>
#include <QString>
#include <QLineEdit>

#include <string>
#include <map>

#ifdef _WIN32
#include <direct.h>
#define getcwd _getcwd // stupid MSFT "deprecation" warning
#else
#include <unistd.h>
#endif

#include <OpenMM.h>

#define NUMSTEPSEXPLICIT 1.5e4;
#define NUMSTEPSIMPLICIT 1e5;

using std::string;
using std::map;

class Simulation {

public:
    Simulation() : stateFile(""), sysFile(""), integratorFile(""), window(nullptr) {};

    // same across all runs
    std::string platform;
    std::string precision;
    
    int deviceId;
    int platformId;
    
    bool verifyAccuracy;
    std::map<std::string, std::string> properties;
    int numSteps;

    // varying per run.
    string stateFile;
    string sysFile;
    string integratorFile;
    
    QLineEdit * window;
    
    map<string, string> getPropertiesMap() const;
};

class SimulationWorker : public QObject {

    Q_OBJECT

public:

    SimulationWorker(QObject *parent = 0);
    ~SimulationWorker();

    template<class T>
    T* loadObject(const string & fname) const;

    double benchmark(OpenMM::Context &context, int numSteps);

public slots:
    void startSimulation(const Simulation & simulation);

signals:
    void emitProgress(QString stateString) const;
    void simulationComplete();

private:
    void updateProgress(const std::string &text) const;
    void loadPlugins(std::string platform);

    bool OpenCLLoaded_;
    bool CUDALoaded_;
    QLineEdit *window_;
};

#endif
