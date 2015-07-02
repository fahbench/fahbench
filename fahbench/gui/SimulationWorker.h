#ifndef SIMULATION_WORKER_H_
#define SIMULATION_WORKER_H_

#include <string>
#include <boost/format.hpp>

#include <QObject>
#include <OpenMM.h>

#include "../Updater.h"
#include "QSimulation.h"

class SimulationWorker : public QObject, public Updater {

    Q_OBJECT

public:
    SimulationWorker();
    void progress(int i, int num_steps, double score);
    void message(std::string);
    void message(boost::format);

public slots:
    void run_simulation(Simulation * simulation);

signals:
    void simulation_finished(const SimulationResult & score);
    void progress_update(const int &, const int &, const double &);
    void message_update(const QString &);
};

#endif
