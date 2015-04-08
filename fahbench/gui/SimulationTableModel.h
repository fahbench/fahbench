#ifndef SIMULATION_TABLE_MODEL_H_
#define SIMULATION_TABLE_MODEL_H_


#include <QString>
#include <QList>
#include <QAbstractTableModel>


class ProteinSystem {
private:
    QString _sysxml;
    QString _intxml;
    QString _statexml;

};

class SimulationTableEntry {
private:
    QString _device;
    QString _platform;
    ProteinSystem _protein;
    // Other options
    float _result;

public:
    QString device() const;
    QString platform() const;
    QString protein() const;
    QString result() const;
};


class SimulationTableModel : public QAbstractTableModel {

private:
    QList<SimulationTableEntry> _entries;


public:
    SimulationTableModel();
    int rowCount(const QModelIndex & parent) const;
    int columnCount(const QModelIndex & parent) const;
    QVariant data(const QModelIndex & index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;

};

#endif
