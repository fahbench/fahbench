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

    SimulationResult _result;

public:
    QString device() const;
    QString platform() const;
    QString protein() const;
    QString result() const;

    void set_result(SimulationResult result);

    SimulationTableEntry(const Device & device)
        : _device(device)
        , _precision("single")
        , _verifyAccuracy(true)
        , _nan_check_freq(1000)
        , _result(ResultStatus::PENDING) {
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
    virtual bool setData(const QModelIndex & index, const QVariant & value, int role = Qt::EditRole);
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    virtual Qt::ItemFlags flags(const QModelIndex & index) const;

    bool has_next() const;
    const SimulationTableEntry & get_next();
    void finish(SimulationResult score);

public slots:
    void simulation_clicked(const QModelIndex & index);

};

#endif
