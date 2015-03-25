
#include "MainWindow.h"
#include "../FAHBenchVersion.h"

#include <sstream>

#include <QThread>

#include <OpenMM.h>

using namespace std;


MainWindow::MainWindow() : QMainWindow()
{
    // Set up SimulationWorker on another thread and connect signals and slots
    worker_ = new SimulationWorker();
    worker_->moveToThread(&thread_);
    connect(&thread_, &QThread::finished, worker_, &QObject::deleteLater);
    connect(this, &MainWindow::doNextSimulation, worker_, &SimulationWorker::startSimulation);
    connect(worker_, &SimulationWorker::simulationComplete, this, &MainWindow::simulationCompleted);

    // Set up layout and add components
    central_widget = new CentralWidget();
    setCentralWidget(central_widget);
    setFixedSize(500,320);
    setWindowTitle(tr(("Folding@Home Benchmark " + getVersion() + " w/ OpenMM " +
                       OpenMM::Platform::getOpenMMVersion()).c_str()));

}

MainWindow::~MainWindow()
{
    thread_.quit();
    thread_.wait();
}

void MainWindow::simulationCompleted() {
    if(simulationQueue_.size() > 0) {
        emit doNextSimulation(simulationQueue_.front());
        simulationQueue_.pop();
    } else {
        central_widget->startButton_->enable();
    }
}


/*
void MainWindow::setupProperties(Simulation &simulation) {

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
*/

void MainWindow::setupSimulation() {
    if(central_widget->qLWidgetHandle_->currentRow() == -1) {
        central_widget->startButton_->enable();
        return;
    }
    Simulation simulation;

    if (central_widget->openCLButton_->isChecked()) {
        simulation.platform = "OpenCL";
    } else if (central_widget->cudaButton_->isChecked()) {
        simulation.platform = "CUDA";
    } else {
        throw(std::runtime_error("No platform selected on the GUI"));
    }
    simulation.verifyAccuracy = central_widget->verifyAccuracyBox_->isChecked();

    if(central_widget->explicitBox_->isChecked()) {
        simulation.solvent = "explicit";
    }

    if (central_widget->implicitBox_->isChecked()) {
        simulation.solvent = "implicit";
    }

    simulationQueue_.push(simulation);



    //setupProperties(genericSimulation);
    /*
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
        }*/

    central_widget->implicitResult_->setText("");
    central_widget->explicitResult_->setText("");

    thread_.start();

    /*
    if(simulationQueue_.size() > 0) {

        emit doNextSimulation(simulationQueue_.front());
        simulationQueue_.pop();
    } else {
        simulationThread_->quit();
        startButton_->enable();
    } */
}


#include "MainWindow.moc"
