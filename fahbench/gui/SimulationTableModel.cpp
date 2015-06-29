#include "SimulationTableModel.h"


const static int NCOLUMNS = 5;
const static int RESULTS_COL = 4;

SimulationTableModel::SimulationTableModel(DeviceTableModel * device_table_model)
    : _entries()
    , current_entry(-1) {
    auto devices = device_table_model->entries();
    for (auto & it : devices) {
        _entries.append(SimulationTableEntry(it));
        current_entry = 0;
    }
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

bool SimulationTableModel::has_next() const {
    return current_entry >= 0;
}


const SimulationTableEntry & SimulationTableModel::get_next() {
    auto & entry = _entries[current_entry];
    entry.set_result(0.0);
    emit dataChanged(index(current_entry, RESULTS_COL), index(current_entry, RESULTS_COL));
    return _entries.at(current_entry);
}

void SimulationTableModel::finish(double score) {
    SimulationTableEntry & entry = _entries[current_entry];
    entry.set_result(score);
    emit dataChanged(index(current_entry, RESULTS_COL), index(current_entry, RESULTS_COL));

    current_entry += 1;
    if (current_entry >= _entries.size()) {
        current_entry = -1;
    }
}


void SimulationTableEntry::configure_simulation(Simulation & sim) {
    sim.platform = _device.platform();
    sim.deviceId = _device.device_id();
    sim.platformId = _device.platform_id();
    sim.precision = _precision.toStdString();
    sim.verifyAccuracy = _verifyAccuracy;
    sim.nan_check_freq = _nan_check_freq;
    auto wu = new WorkUnit(std::string("dhfr-implicit"));
    wu->set_n_steps(500); // TODO: this is small for debugging
    sim.work_unit = wu;
}


QString SimulationTableEntry::device() const {
    auto device_name = QString::fromStdString(_device.device());
    if (_device.platform() == "OpenCL") {
        return QString("%1 (%2, %3)").arg(device_name).arg(_device.platform_id()).arg(_device.device_id());
    } else {
        return QString("%1 (%2)").arg(device_name).arg(_device.device_id());
    }

}
QString SimulationTableEntry::platform() const {
    return QString::fromStdString(_device.platform());
}
QString SimulationTableEntry::protein() const {
    return QString("Placeholder");
}
QString SimulationTableEntry::result() const {
    if (_result > 0) {
        return QString("%1 ns/day").arg(_result);
    } else if (_result < 0) {
        return QString("(Pending)");
    } else {
        return QString("(Benchmarking...)");
    }
}

void SimulationTableEntry::set_result(double result) {
    _result = result;
}

#include "SimulationTableModel.moc"