#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include "CentralWidget.h"
#include "SimulationWorker.h"
#include "QSimulation.h"

#include <string>
#include <map>
#include <queue>

#include <QMainWindow>
#include <QThread>
#include <QMenuBar>
#include <QAction>



class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow();
    ~MainWindow();

protected:
	void showEvent(QShowEvent * event);

private:
    CentralWidget * central_widget;

    QThread thread;
    SimulationWorker * worker;

    void make_actions();
    void make_menu_bar();

    QAction * exit_action;
    QAction * about_action;

signals:
    void start_new_simulation(const Simulation &);

private slots:
    void simulation_finished(const SimulationResult & score);
    void start_button_clicked();
    void interrupt_simulation();

    void about();

};


#endif // MAIN_WINDOW_H
