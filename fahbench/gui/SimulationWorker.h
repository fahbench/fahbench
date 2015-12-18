#ifndef SIMULATION_WORKER_H_
#define SIMULATION_WORKER_H_

#include "../Updater.h"
#include "QSimulation.h"

#include <string>
#include <boost/format.hpp>

#include <QObject>
#include <QString>


class SimulationWorker : public QObject, public Updater {

    Q_OBJECT

public:
    SimulationWorker();
    void progress(int i, int num_steps, float score);
    void message(std::string);
    void message(boost::format);

public slots:
    void run_simulation(const Simulation & simulation);

signals:
    void simulation_finished(const SimulationResult & score);
    void progress_update(const int &, const int &, const float &);
    void message_update(const QString &);
};

#endif
