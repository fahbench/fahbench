
#include "MainWindow.h"
#include "../FAHBenchVersion.h"

#include <sstream>
#include <QThread>
#include <OpenMM.h>

#include <QMessageBox>

using namespace std;


MainWindow::MainWindow() : QMainWindow() {
    qRegisterMetaType<Simulation>();

    // Set up SimulationWorker on another thread and connect signals and slots
    worker = new SimulationWorker();
    worker->moveToThread(&thread);
    connect(&thread, &QThread::finished, worker, &QObject::deleteLater);
    connect(this, &MainWindow::start_new_simulation, worker, &SimulationWorker::run_simulation);
    connect(worker, &SimulationWorker::simulation_finished, this, &MainWindow::simulation_finished);
    thread.start();

    // Set up layout and add components
    central_widget = new CentralWidget();
    setCentralWidget(central_widget);
    connect(central_widget->start_button, &QAbstractButton::clicked, this, &MainWindow::start_button_clicked);
    connect(worker, &SimulationWorker::progress_update, central_widget, &CentralWidget::progress_update);
    connect(worker, &SimulationWorker::message_update, central_widget, &CentralWidget::message_update);
    setWindowTitle("FAHBench");

    make_actions();
    make_menu_bar();
}

MainWindow::~MainWindow() {
    thread.quit();
    thread.wait();
}

void MainWindow::make_actions() {
    about_action = new QAction("About", this);
    connect(about_action, SIGNAL(triggered(bool)), this, SLOT(about()));

    exit_action = new QAction("Exit", this);
    connect(exit_action, SIGNAL(triggered(bool)), this, SLOT(close()));
}


void MainWindow::make_menu_bar() {
    auto file_menu = menuBar()->addMenu("&File");
    file_menu->addAction(exit_action);
    auto help_menu = menuBar()->addMenu("Help");
    help_menu->addAction(about_action);
}


void MainWindow::start_button_clicked() {
    Simulation sim;
    // TODO: Get from GUI
    sim.platform = "OpenCL";
    sim.deviceId = 0;
    sim.precision = "single";
    sim.platformId = 0;
    sim.verifyAccuracy = false;
    sim.nan_check_freq = 1000;
    sim.numSteps = 5000;
    sim.solvent = "explicit";

    auto pbar = central_widget->progress_bar;
    auto sbut = central_widget->start_button;
    pbar->reset();
    // Show "busy" bar
    pbar->setMinimum(0);
    pbar->setMaximum(0);
    sbut->setEnabled(false);
    emit start_new_simulation(sim);
}

void MainWindow::simulation_finished(const double &) {
    auto pbar = central_widget->progress_bar;
    auto sbut = central_widget->start_button;
    pbar->setValue(pbar->maximum());
    sbut->setEnabled(true);
}

void MainWindow::about() {
    QString about_text("Folding @ Home Benchmark\n");
    about_text += "version " + QString::fromStdString(getVersion()) + " \n";
    QMessageBox::about(this, "FAHBench", about_text);
}


#include "MainWindow.moc"
