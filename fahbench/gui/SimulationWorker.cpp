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


void SimulationWorker::startSimulation(const Simulation & simulation) {

    try {
        simulation.run(*this);
    } catch(std::exception &e) {
        message(e.what());
        emit simulationComplete();
    }
}

void SimulationWorker::progress(int i)
{
    // TODO
    message(boost::format("Progress update: %1%") % i);
}

void SimulationWorker::message(std::string s)
{
    emit emitProgress(QString::fromStdString(s));

}

void SimulationWorker::message(boost::format f)
{
    emit emitProgress(QString::fromStdString(f.str()));

}

#include "SimulationWorker.moc"


