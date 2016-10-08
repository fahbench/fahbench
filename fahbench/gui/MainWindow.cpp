#include <mutex>
#include <OpenMM.h>

#include "MainWindow.h"
#include "FAHBenchVersion.h"

#include <QMessageBox>
#include <QDebug>

using namespace std;

MainWindow::MainWindow() : QMainWindow() {
    qRegisterMetaType<Simulation>();
    qRegisterMetaType<SimulationResult>();

    // Set up SimulationWorker on another thread and connect signals and slots
    worker = new SimulationWorker();
    worker->moveToThread(&thread);
    // Initialize central widget
    central_widget = new CentralWidget();

    // Wire everything up
    connect(&thread, &QThread::finished, worker, &QObject::deleteLater);
    connect(this, &MainWindow::start_new_simulation, worker, &SimulationWorker::run_simulation);
    connect(worker, &SimulationWorker::simulation_finished, this, &MainWindow::simulation_finished);
    connect(central_widget->cancel_button, SIGNAL(clicked()), this, SLOT(interrupt_simulation()));
    connect(central_widget->start_button, &QAbstractButton::clicked, this, &MainWindow::start_button_clicked);
    connect(worker, &SimulationWorker::progress_update, central_widget, &CentralWidget::progress_update);
    connect(worker, &SimulationWorker::message_update, central_widget, &CentralWidget::message_update);

    make_actions();
    make_menu_bar();
    setWindowTitle("FAHBench");
    setCentralWidget(central_widget);

    thread.start();
}

MainWindow::~MainWindow() {
    interrupt_simulation();
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
    central_widget->configure_simulation(sim);

    auto pbar = central_widget->progress_bar;
    auto sbut = central_widget->start_button;
    auto cbut = central_widget->cancel_button;

    std::lock_guard<std::mutex> lock(worker->cancelled_mutex);
    worker->is_cancelled = false;

    pbar->reset();
    // Show "busy" bar
    pbar->setMinimum(0);
    pbar->setMaximum(0);
    sbut->setEnabled(false);
    cbut->setEnabled(true);
    emit start_new_simulation(sim);
}

void MainWindow::simulation_finished(const SimulationResult & score) {
    auto pbar = central_widget->progress_bar;
    auto sbut = central_widget->start_button;
    auto cbut = central_widget->cancel_button;
    pbar->setMinimum(0);
    pbar->setMaximum(1);
    pbar->setValue(pbar->maximum());
    sbut->setEnabled(true);
    cbut->setEnabled(false);
    cbut->setText("Cancel");

    qDebug() << score.score();
    qDebug() << score.scaled_score();
    central_widget->results_wid->set_result(score);
}

void MainWindow::interrupt_simulation() {
    auto cbut = central_widget->cancel_button;
    cbut->setEnabled(false);
    cbut->setText("Cancelling...");

    std::lock_guard<std::mutex> lock(worker->cancelled_mutex);
    worker->is_cancelled = true;
}

inline std::string getOpenMMVersion() {
    return OpenMM::Platform::getOpenMMVersion();
}

void MainWindow::about() {
    QString about_text(
        "<h1>FAHBench</h1>"
        "<h3>GPU Benchmarking for Folding@home</h3>"
    );
    about_text += "<p>";
    about_text += "Version " + QString::fromStdString(getVersion());
    about_text += "<br />";
    about_text += "OpenMM version " + QString::fromStdString(getOpenMMVersion());
    about_text += "</p>";
    QMessageBox::about(this, "About FAHBench", about_text);
}


#include "MainWindow.moc"
