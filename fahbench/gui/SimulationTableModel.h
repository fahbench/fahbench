#ifndef SIMULATION_TABLE_MODEL_H_
#define SIMULATION_TABLE_MODEL_H_


#include <QString>
#include <QList>
#include <QAbstractTableModel>

#include "QSimulation.h"
#include "DeviceTableModel.h"
#include "../Device.h"
#include "../WorkUnit.h"

class SimulationTableEntry {

private:
    Device _device;

    QString _precision;
    bool _verifyAccuracy;
    int _nan_check_freq;

    double _result;

public:
    QString device() const;
    QString platform() const;
    QString protein() const;
    QString result() const;

    void set_result(double result);

    SimulationTableEntry(const Device & device)
        : _device(device)
        , _precision("single")
        , _verifyAccuracy(true)
        , _nan_check_freq(1000)
        , _result(-1.0) {
    }

    void configure_simulation(Simulation & sim);

};


class SimulationTableModel : public QAbstractTableModel {
    Q_OBJECT

private:
    QList<SimulationTableEntry> _entries;
    int current_entry;


public:
    SimulationTableModel(DeviceTableModel * device_table_model);
    int rowCount(const QModelIndex & parent) const;
    int columnCount(const QModelIndex & parent) const;
    QVariant data(const QModelIndex & index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;

    bool has_next() const;
    const SimulationTableEntry & get_next();
    void finish(double score);

};

#endif
