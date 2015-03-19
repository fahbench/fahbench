
#include "window.h"
#include "OneShotButton.h"
#include "FAHBenchVersion.h"
#include <sstream>

#include <QGridLayout>
#include <QBoxLayout>
#include <QLabel>
#include <QThread>

#include <OpenMM.h>

using namespace std;

#include <QMetaType>

Q_DECLARE_METATYPE(Simulation);

Window::Window(QWidget *parent)
    : QWidget(parent),
      simulation_(new SimulationWorker()),
      simulationThread_(new QThread()) {
    QGridLayout *grid = new QGridLayout;
    grid->addWidget(createPlatformsPicker(), 0, 0);
    grid->addWidget(createOptions(), 1, 0);
    grid->addWidget(createAvailableDevices(), 0, 1, 2, 1);
    grid->addWidget(createResults(), 2, 0, 1, 2);
    setLayout(grid);
    setFixedSize(500,320);
    setWindowTitle(tr(("Folding@Home Benchmark " + getVersion() + " w/ OpenMM " + 
        OpenMM::Platform::getOpenMMVersion()).c_str()));
    simulation_->moveToThread(simulationThread_);
    qRegisterMetaType<Simulation>("Simulation");
    connect(this, SIGNAL(doNextSimulation(Simulation)),
            simulation_, SLOT(startSimulation(Simulation)));
}

// using layouts:
// first we create buttons and add them to a layout
// we then make another container widget (eg. QGroupBox) and set its layout
// to that of the layout we just made
// then we return the widget

QGroupBox *Window::createPlatformsPicker() {
    openCLButton_ = new QRadioButton(tr("OpenCL - F@H"));
    cudaButton_ = new QRadioButton(tr("CUDA - OpenMM"));
    connect(openCLButton_, SIGNAL(clicked()), this, SLOT(updateOpenCLDevices()));
    connect(cudaButton_, SIGNAL(clicked()), this, SLOT(updateCUDADevices()));
    openCLButton_->setChecked(true);
    QVBoxLayout *vbox = new QVBoxLayout;
    vbox->addWidget(openCLButton_);
    vbox->addWidget(cudaButton_);
    vbox->addStretch(1);
    QGroupBox *groupBox = new QGroupBox(tr("Platform"));
    groupBox->setLayout(vbox);
    return groupBox;
}

QGroupBox *Window::createOptions() {
    explicitBox_ = new QCheckBox(tr("Explicit"));
    explicitBox_->setChecked(true);
    implicitBox_ = new QCheckBox(tr("Implicit"));
    implicitBox_->setChecked(true);
    precisionBox_ = new QCheckBox(tr("Double Precision"));
    verifyAccuracyBox_ = new QCheckBox(tr("Verify Accuracy"));
    verifyAccuracyBox_->setChecked(false);
    stressTestBox_ = new QCheckBox(tr("Stress Test"));
	stressTestBox_->setChecked(false);

    QVBoxLayout *vbox = new QVBoxLayout;
    vbox->addWidget(explicitBox_);
    vbox->addWidget(implicitBox_);
    vbox->addWidget(verifyAccuracyBox_);
    vbox->addWidget(precisionBox_);
    vbox->addWidget(stressTestBox_);
    vbox->addStretch(1);
    QGroupBox *groupBox = new QGroupBox(tr("Options"));
    groupBox->setLayout(vbox);
    return groupBox;
}
void Window::updateCUDADevices() {
    qLWidgetHandle_->clear();
    try{
        //cudaDeviceMap_ = GPUInfo::getCUDADevices();
        if(cudaDeviceMap_.size() == 0) {
            qLWidgetHandle_->addItem("No CUDA devices found.");
            qLWidgetHandle_->setSelectionMode(QAbstractItemView::NoSelection);
            return;
        }
        for(auto it=cudaDeviceMap_.begin(); it != cudaDeviceMap_.end(); it++) {
            qLWidgetHandle_->addItem(it->first.c_str());
        }
        qLWidgetHandle_->setSelectionMode(QAbstractItemView::ExtendedSelection);
        qLWidgetHandle_->setCurrentRow(0);
    } catch(const std::exception &e) {
        qLWidgetHandle_->addItem(e.what());
        qLWidgetHandle_->setCurrentRow(0);
        qLWidgetHandle_->setSelectionMode(QAbstractItemView::NoSelection);
    }
}

void Window::updateOpenCLDevices() {
    //openCLDeviceMap_ = GPUInfo::getOpenCLDevices();
    qLWidgetHandle_->clear();
    for(auto it = openCLDeviceMap_.begin(); it != openCLDeviceMap_.end(); it++) {
        qLWidgetHandle_->addItem(it->first.c_str());
    }
    qLWidgetHandle_->setSelectionMode(QAbstractItemView::ExtendedSelection);
    qLWidgetHandle_->setCurrentRow(0);
}

void Window::simulationCompleted() {
    if(simulationQueue_.size() > 0) {
        emit doNextSimulation(simulationQueue_.front());
        simulationQueue_.pop();
    } else {
        simulationThread_->quit();
        startButton_->enable();
        disconnect(simulation_, SIGNAL(simulationComplete()), this, SLOT(simulationCompleted()));
    }
}

#include <iostream>

void Window::setupProperties(Simulation &simulation) {
    assert(simulation.platform.size() > 0);
    // setup device indices
    string prefix;
    if(openCLButton_->isChecked()) {
        prefix = "OpenCL";

        QList<QListWidgetItem *> selection = qLWidgetHandle_->selectedItems();
        stringstream dIdstream;
        stringstream pIdstream;
        for(int i=0; i<selection.size(); i++) {
            string deviceName = selection[i]->text().toStdString();
            dIdstream << openCLDeviceMap_[deviceName];
            if(i == 0) {
                pIdstream << openCLDeviceMap_[deviceName];
            }
            if(i != selection.size() - 1) {
                dIdstream << ',';
            }
        }


        simulation.properties["OpenCLDeviceIndex"]=dIdstream.str();
        simulation.properties["OpenCLPlatformIndex"]=pIdstream.str();
    } else if(cudaButton_->isChecked()) {
        prefix = "Cuda";
        string deviceName = qLWidgetHandle_->currentItem()->text().toStdString();
        stringstream dIndex; dIndex << cudaDeviceMap_[deviceName];
        simulation.properties["CudaDeviceIndex"]=dIndex.str();
    }
    if(precisionBox_->isChecked()) {
        simulation.properties[prefix+"Precision"] = "double";
    }
}

void Window::setupSimulation() {
    if(qLWidgetHandle_->currentRow() == -1) {
        startButton_->enable();
        return;
    }
    Simulation genericSimulation;
    genericSimulation.numSteps = 0;
    if(openCLButton_->isChecked()) {
        genericSimulation.platform = "OpenCL";
    } else if(cudaButton_->isChecked()) {
        genericSimulation.platform = "CUDA";
    } else {
        throw(std::runtime_error("No platform selected on the GUI"));
    }
    genericSimulation.verifyAccuracy = verifyAccuracyBox_->isChecked();
    setupProperties(genericSimulation);

    // Put the runs into a queue
    if(explicitBox_->isChecked()) {
        Simulation simulation(genericSimulation);
        simulation.numSteps  = NUMSTEPSEXPLICIT;
        simulation.window    = explicitResult_;
        simulation.sysFile   = "./scratch/DHFR_SYSTEM_EXPLICIT.xml";
        simulation.stateFile = "./scratch/DHFR_STATE_EXPLICIT.xml";
        simulation.integratorFile = "./scratch/DHFR_INTEGRATOR_EXPLICIT.xml";
		if(stressTestBox_->isChecked()) {
            simulation.numSteps = simulation.numSteps*10;
        }
        simulationQueue_.push(simulation);
    }

    if(implicitBox_->isChecked()) {
        Simulation simulation(genericSimulation);
        simulation.numSteps  = NUMSTEPSIMPLICIT;
        simulation.window    = implicitResult_;
        simulation.sysFile   = "./scratch/DHFR_SYSTEM_IMPLICIT.xml";
        simulation.stateFile = "./scratch/DHFR_STATE_IMPLICIT.xml";
        simulation.integratorFile = "./scratch/DHFR_INTEGRATOR_IMPLICIT.xml";
		if(stressTestBox_->isChecked()) {
            simulation.numSteps = simulation.numSteps*10;
        }
        simulationQueue_.push(simulation);
    }

    implicitResult_->setText("");
    explicitResult_->setText("");

    simulationThread_->start();

    if(simulationQueue_.size() > 0) {
        connect(simulation_, SIGNAL(simulationComplete()), this, SLOT(simulationCompleted()));
        emit doNextSimulation(simulationQueue_.front());
        simulationQueue_.pop();
    } else {
        simulationThread_->quit();
        startButton_->enable();
    }

}
QGroupBox *Window::createAvailableDevices() {
    QGroupBox *groupBox = new QGroupBox(tr("Available Devices"));
    //groupBox->setFlat(true);
    qLWidgetHandle_ = new QListWidget;
    updateOpenCLDevices();
    QVBoxLayout *vbox = new QVBoxLayout;
    vbox->addWidget(qLWidgetHandle_);
    vbox->addStretch(1);
    groupBox->setLayout(vbox);
    return groupBox;
}

QGroupBox *Window::createResults() {
    QLabel *explicitLabel = new QLabel("Explicit Solvent: ");
    QLabel *implicitLabel = new QLabel("Implicit Solvent: ");
    QGridLayout *gLayout = new QGridLayout(this);
    gLayout->addWidget(explicitLabel,0,0);
    gLayout->addWidget(implicitLabel,1,0);
  
    explicitResult_ = new QLineEdit();
    explicitResult_->setReadOnly(true);
    implicitResult_ = new QLineEdit();
    implicitResult_->setReadOnly(true);
    
    gLayout->addWidget(explicitResult_,0,1);
    gLayout->addWidget(implicitResult_,1,1);

    startButton_ = new OneShotButton(tr("&Start"), this);
    connect(startButton_, SIGNAL(clicked()), this, SLOT(setupSimulation()));
    startButton_->setMinimumWidth(50);
    startButton_->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    gLayout->addWidget(startButton_, 0,2,2,1);

    gLayout->setColumnStretch(0, 15);
    gLayout->setColumnStretch(1, 85);
    gLayout->setColumnStretch(2, 10);

    QGroupBox *groupBox = new QGroupBox(tr("&Results"));
    groupBox->setLayout(gLayout);
    return groupBox;
}