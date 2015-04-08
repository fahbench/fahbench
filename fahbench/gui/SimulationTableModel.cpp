#include "SimulationTableModel.h"

const static int NCOLUMNS = 5;

SimulationTableModel::SimulationTableModel() :
    _entries() {
}

int SimulationTableModel::rowCount(const QModelIndex & parent) const {
    return _entries.size();
}

int SimulationTableModel::columnCount(const QModelIndex & parent) const {
    return NCOLUMNS;
}

QVariant SimulationTableModel::data(const QModelIndex & index, int role) const {
    if (!index.isValid())
        return QVariant();

    if (index.row() >= _entries.size())
        return QVariant();

    if (role != Qt::DisplayRole)
        return QVariant();

    auto item = _entries.at(index.row());
    switch (index.column()) {
    case 0:
        return item.device();
    case 1:
        return item.platform();
    case 2:
        return item.protein();
    case 3:
        return QString("Advanced");
    case 4:
        return item.result();
    default:
        return QVariant();
    }
}


QVariant SimulationTableModel::headerData(int section, Qt::Orientation orientation, int role) const {
    if (role != Qt::DisplayRole) {
        return QVariant();
    }

    if (orientation == Qt::Horizontal) {
        switch (section) {
        case 0:
            return QString("Device");
        case 1:
            return QString("Platform");
        case 2:
            return QString("Protein");
        case 3:
            return QString("Advanced");
        case 4:
            return QString("Results");
        default:
            return QVariant();
        }
    } else {
        return QString("%1").arg(section);
    }
}


QString SimulationTableEntry::device() const {
    return _device;
}
QString SimulationTableEntry::platform() const {
    return _platform;
}
QString SimulationTableEntry::protein() const {
    return QString("Protein placeholder");
}
QString SimulationTableEntry::result() const {
    return QString("%1 ns/day").arg(_result);
}

#include "SimulationTableModel.moc"