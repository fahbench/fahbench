
#include "MainWindow.h"
#include "../FAHBenchVersion.h"

#include <sstream>

#include <QThread>

#include <OpenMM.h>

using namespace std;


MainWindow::MainWindow() : QMainWindow()
{
    // Set up SimulationWorker on another thread and connect signals and slots
    worker = new SimulationWorker();
    worker->moveToThread(&thread);
    connect(&thread, &QThread::finished, worker, &QObject::deleteLater);
    connect(this, &MainWindow::start_new_simulation, worker, &SimulationWorker::run_simulation);
    connect(worker, &SimulationWorker::simulation_finished, this, &MainWindow::simulation_finished);

    // Set up layout and add components
    central_widget = new CentralWidget();
    setCentralWidget(central_widget);
	setWindowTitle("FAHBench");

}

MainWindow::~MainWindow()
{
    thread.quit();
    thread.wait();
}

void MainWindow::simulation_finished(Simulation & sim) {
	/*
    if(simulationQueue_.size() > 0) {
        emit doNextSimulation(simulationQueue_.front());
        simulationQueue_.pop();
    } else {
        //central_widget->startButton_->enable();
    }
	*/
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



#include "MainWindow.moc"
