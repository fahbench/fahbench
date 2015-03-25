#ifndef SIMULATION_WORKER_H_
#define SIMULATION_WORKER_H_

#include <QObject>
#include <QString>
#include <QLineEdit>

#include <string>
#include <sstream>
#include <map>
#include <boost/format.hpp>

#ifdef _WIN32
#include <direct.h>
#define getcwd _getcwd // stupid MSFT "deprecation" warning
#else
#include <unistd.h>
#endif

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
    
    QLineEdit * window;
    
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

class SimulationWorker : public QObject, public Updater {

    Q_OBJECT

public:
    SimulationWorker(QObject *parent = 0);
    void progress(int);
    void message(std::string);
    void message(boost::format);

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
