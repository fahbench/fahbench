
#include "SimulationWorker.h"
#include <exception>

using std::string;
using std::map;

SimulationWorker::SimulationWorker()
    : QObject()
    , is_cancelled(false)
    , cancelled_mutex() {
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


void SimulationWorker::progress(int i, int num_steps, float score) const {
    emit progress_update(i, num_steps, score);
}

void SimulationWorker::message(std::string s) const {
    emit message_update(QString::fromStdString(s));

}

void SimulationWorker::message(boost::format f) const {
    emit message_update(QString::fromStdString(f.str()));
}

bool SimulationWorker::cancelled() const {
    bool ret = false;
    if (cancelled_mutex.try_lock()) {
        ret = is_cancelled;
        cancelled_mutex.unlock();
    }
    if (ret) {
        emit message_update("Cancelled!");
    }
    return ret;
}

#include "SimulationWorker.moc"


