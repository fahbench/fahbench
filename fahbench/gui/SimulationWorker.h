#ifndef SIMULATION_WORKER_H_
#define SIMULATION_WORKER_H_

#include <string>
#include <sstream>
#include <map>
#include <boost/format.hpp>

#include <QObject>

#include <OpenMM.h>

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
    void run_simulation(Simulation & simulation);

signals:
    void simulation_finished(Simulation & simulation);
    void progress_update(const int &);
    void message_update(const QString &);

};

#endif
