#ifndef SIMULATION_WORKER_H_
#define SIMULATION_WORKER_H_

#include "../Updater.h"
#include "QSimulation.h"

#include <string>
#include <boost/format.hpp>
#include <mutex>

#include <QObject>
#include <QString>


class SimulationWorker : public QObject, public Updater {

    Q_OBJECT


private:
    bool _cancelled;
    mutable std::mutex cancelled_mutex;

public:
    SimulationWorker();
    void progress(int i, int num_steps, float score) const;
    void message(std::string) const;
    void message(boost::format) const;
    bool cancelled() const;

public slots:
    void run_simulation(const Simulation & simulation);
    void interrupt_simulation();

signals:
    void simulation_finished(const SimulationResult & score);
    void progress_update(const int &, const int &, const float &) const;
    void message_update(const QString &) const;
};

#endif
