#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H
#include <string>
#include <map>


#include <QMainWindow>
#include <QThread>

#include <queue>

#include "CentralWidget.h"
#include "SimulationWorker.h"
#include "../Simulation.h"




class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow();
    virtual ~MainWindow();
    
    // prepares a list of jobs and inserts them into the
    // simulationQueue_ data structure
    void setupSimulation();

public slots:
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
    CentralWidget * central_widget;
    QThread thread_;
    SimulationWorker * worker_;
    std::queue<Simulation> simulationQueue_;

};


#endif // MAIN_WINDOW_H
