#include <iostream>
#include <sstream>
#include <fstream>
#include <string>

#include <stdexcept>
#include <boost/format.hpp>

#include <QString>

#include "SimulationWorker.h"

#ifdef WIN32
#include <float.h>
#define isnan(x) _isnan(x)
#endif

using namespace std;
using namespace OpenMM;

using std::string;
using std::map;

SimulationWorker::SimulationWorker(): QObject() {
    qRegisterMetaType<Simulation>();
}


void SimulationWorker::run_simulation(Simulation simulation) {
    std::cout << "Caught run simulation\n";
    try {
        double score = simulation.run(*this);
        emit simulation_finished(score);
    } catch (std::exception & e) {
        message(e.what());
        emit simulation_finished(0.0);
    }
}

void SimulationWorker::progress(int i, int num_steps, double score) {
    emit progress_update(i, num_steps, score);
}

void SimulationWorker::message(std::string s) {
    emit message_update(QString::fromStdString(s));

}

void SimulationWorker::message(boost::format f) {
    emit message_update(QString::fromStdString(f.str()));
}

#include "SimulationWorker.moc"


