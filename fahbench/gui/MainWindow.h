#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <string>
#include <map>
#include <queue>

#include <QMainWindow>
#include <QThread>
#include <QMenuBar>
#include <QAction>

#include "CentralWidget.h"
#include "SimulationWorker.h"
#include "QSimulation.h"


class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow();
    ~MainWindow();

private:
    CentralWidget * central_widget;

    QThread thread;
    SimulationWorker * worker;

    void make_actions();
    void make_menu_bar();

    QAction * about_action;

signals:
    void start_new_simulation(Simulation);

private slots:
    void simulation_finished(const double &);
    void start_button_clicked();

    void about();

};


#endif // MAIN_WINDOW_H
