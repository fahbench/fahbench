#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <string>
#include <map>
#include <queue>

#include <QMainWindow>
#include <QThread>

#include "CentralWidget.h"
#include "SimulationWorker.h"
#include "../Simulation.h"


class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow();
    ~MainWindow();

private:
    CentralWidget * central_widget;
    QThread thread;
    SimulationWorker * worker;

signals:
    void start_new_simulation(Simulation &);

private slots:
    void simulation_finished(Simulation &);

};


#endif // MAIN_WINDOW_H
