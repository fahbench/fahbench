#ifndef SIMULATION_WORKER_H_
#define SIMULATION_WORKER_H_

#include <string>
#include <sstream>
#include <map>
#include <boost/format.hpp>

#include <QObject>

#include <OpenMM.h>

//#include "MainWindow.h"
#include "../Updater.h"
#include "../Simulation.h"

using std::string;
using std::map;



class SimulationWorker : public QObject, public Updater {

    Q_OBJECT

public:
    void progress(int);
    void message(std::string);
    void message(boost::format);

public slots:
    void startSimulation(const Simulation & simulation);

signals:
    void emitProgress(QString stateString) const;
    void simulationComplete();

};

#endif
