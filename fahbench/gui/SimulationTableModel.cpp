#include "SimulationTableModel.h"


const static int NCOLUMNS = 5;
const static int RESULTS_COL = 4;

SimulationTableModel::SimulationTableModel() :
    _entries(),
    current_entry(-1) {
    _entries.append(SimulationTableEntry());
    current_entry = 0;
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
    case RESULTS_COL:
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
        case RESULTS_COL:
            return QString("Results");
        default:
            return QVariant();
        }
    } else {
        return QString("%1").arg(section);
    }
}

const SimulationTableEntry & SimulationTableModel::get_next() {
    return _entries.at(current_entry);
}

void SimulationTableModel::finish(double score) {
    auto entry = &_entries[current_entry];
    entry->set_result(score);
    emit dataChanged(index(current_entry, RESULTS_COL), index(current_entry, RESULTS_COL));
}


void SimulationTableEntry::configure_simulation(Simulation & sim) {

    sim.platform = _platform.toStdString();
    sim.deviceId = _deviceId;
    sim.precision = _precision.toStdString();
    sim.platformId = _platformId;
    sim.verifyAccuracy = _verifyAccuracy;
    sim.nan_check_freq = _nan_check_freq;
    sim.numSteps = _num_steps;

    // TODO: figure out how to pass xml files.
    sim.solvent = "explicit";
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

void SimulationTableEntry::set_result(double result) {
    _result = result;
}

#include "SimulationTableModel.moc"