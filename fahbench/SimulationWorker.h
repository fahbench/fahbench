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
#elif
#include <unistd.h>
#endif

#include <OpenMM.h>

#define NUMSTEPSEXPLICIT 1.5e4;
#define NUMSTEPSIMPLICIT 1e5;

// To Do: abstract out QLineEdit into a generic string slot

struct Simulation {

    Simulation() : platform(), verifyAccuracy(true), properties(), numSteps(0), 
        stateFile(NULL), sysFile(NULL), integratorFile(NULL), window(NULL) {};

    // same across all runs
    QString platform;
    bool verifyAccuracy;
    std::map<std::string, std::string> properties;
    int numSteps;
    
    // varying per run.
    char *stateFile;
    char *sysFile;
    char *integratorFile;
    QLineEdit* window;
};

class SimulationWorker : public QObject {

    Q_OBJECT
           
public:
    
    SimulationWorker(QObject *parent = 0);
    ~SimulationWorker();
    
    template<class T> 
    T* loadObject(const char *fname) const;
    
    double benchmark(OpenMM::Context &context, int numSteps);

public slots:
    void startSimulation(Simulation simulation);

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