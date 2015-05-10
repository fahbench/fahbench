#ifndef SIMULATION_TABLE_MODEL_H_
#define SIMULATION_TABLE_MODEL_H_


#include <QString>
#include <QList>
#include <QAbstractTableModel>

#include "QSimulation.h"


class ProteinSystem {
private:
    QString _sysxml;
    QString _intxml;
    QString _statexml;

};

class SimulationTableEntry {

private:
    QString _device; // TODO: Should we store this as a string?
    QString _platform;
    ProteinSystem _protein;

    int _deviceId;
    int _platformId;
    QString _precision;
    bool _verifyAccuracy;
    int _nan_check_freq;
    int _num_steps;

    double _result;

public:
    QString device() const;
    QString platform() const;
    QString protein() const;
    QString result() const;

    void set_result(double result);

    SimulationTableEntry() :
        _platform("OpenCL"),
        _device("Device display name"),
        _protein(ProteinSystem()),
        _deviceId(0),
        _platformId(0),
        _precision("single"),
        _verifyAccuracy(true),
        _nan_check_freq(1000),
        _num_steps(5000),
        _result(0.0) {
    }

    void configure_simulation(Simulation & sim);

};


class SimulationTableModel : public QAbstractTableModel {
    Q_OBJECT

private:
    QList<SimulationTableEntry> _entries;
    int current_entry;


public:
    SimulationTableModel();
    int rowCount(const QModelIndex & parent) const;
    int columnCount(const QModelIndex & parent) const;
    QVariant data(const QModelIndex & index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;

    const SimulationTableEntry & get_next();
    void finish(double score);

};

#endif
