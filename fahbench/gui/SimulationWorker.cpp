
#include "SimulationWorker.h"
#include <exception>



using std::string;
using std::map;

SimulationWorker::SimulationWorker(): QObject() {
    qRegisterMetaType<Simulation>();
    qRegisterMetaType<SimulationResult>();
}


void SimulationWorker::run_simulation(const Simulation & simulation) {
    try {
        SimulationResult score = simulation.run(*this);
        emit simulation_finished(score);
    } catch (std::exception & e) {
        message(e.what());
        emit simulation_finished(SimulationResult(ResultStatus::FAILED));
    }
}

void SimulationWorker::progress(int i, int num_steps, float score) {
    emit progress_update(i, num_steps, score);
}

void SimulationWorker::message(std::string s) {
    emit message_update(QString::fromStdString(s));

}

void SimulationWorker::message(boost::format f) {
    emit message_update(QString::fromStdString(f.str()));
}

#include "SimulationWorker.moc"


