#ifndef WINDOW_H
#define WINDOW_H
#include <string>
#include <map>

#include <QWidget>
#include <QListWidget>

#include <queue>
#include "OneShotButton.h"

#include "../SimulationWorker.h"
#include "../gpuinfo.h"

QT_BEGIN_NAMESPACE
class QGroupBox;
class QCheckBox;
class QHBoxLayout;
class QRadioButton;
QT_END_NAMESPACE

struct OpenCLId;

class Window : public QWidget {
    Q_OBJECT
public:
    Window(QWidget *parent = 0);

public slots:

    // updates the internal openCLDeviceMap_ object
    // by querying the OpenCL drivers at run-time
    // the available OpenCL devices are vendor specific
    void updateOpenCLDevices();

    // updates the internal cudaDeviceMaps_ object
    // by looking for nvcuda.dll, which is usually located
    // under C:/Windows/System32/nvcuda.dll
    void updateCUDADevices();

    // prepares a list of jobs and inserts them into the
    // simulationQueue_ data structure
    void setupSimulation();

    // this slot receives signals from the SimulationWorker's 
    // simulationComplete() function, it then checks the simulationQueue_ to
    // see if another job should be issued out
    void simulationCompleted();

signals:
    // signals to one of the workerThreads to commence the subsequent
    // simulation
    // replaced with start() thread
    void doNextSimulation(Simulation);

private:

    // based on the options chosen by the user, set the properties mapping
    // for OpenMM
    void setupProperties(Simulation &simulation);

    QGroupBox *createPlatformsPicker(); 
    QGroupBox *createOptions();
    QGroupBox *createAvailableDevices();
    QGroupBox *createResults();

    // Device List
    QListWidget *qLWidgetHandle_;

    // Options
    QCheckBox *explicitBox_;
    QCheckBox *implicitBox_;
    QCheckBox *precisionBox_;
    QCheckBox *verifyAccuracyBox_;
    QCheckBox *stressTestBox_;

    // Platform Buttons
    QRadioButton *cudaButton_;
    QRadioButton *openCLButton_;

    // Result Boxes
    QLineEdit *explicitResult_;
    QLineEdit *implicitResult_;

    std::map<std::string, OpenCLId> openCLDeviceMap_;
    std::map<std::string, int> cudaDeviceMap_;

    QThread *simulationThread_;
    SimulationWorker *simulation_;
    std::queue<Simulation> simulationQueue_;

    OneShotButton *startButton_;
};
//! [0]

#endif
