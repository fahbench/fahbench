#include "DeviceTableModel.h"

const static int NCOLUMNS = 4;

DeviceTableModel::DeviceTableModel() :
    _entries() {
    auto opencl_devices = GPUInfo::getOpenCLDevices();
    auto cuda_devices = GPUInfo::getCUDADevices();
    _entries.insert(_entries.end(), opencl_devices.begin(), opencl_devices.end());
    _entries.insert(_entries.end(), cuda_devices.begin(), cuda_devices.end());
}

int DeviceTableModel::rowCount(const QModelIndex & parent) const {
    return _entries.size();
}

int DeviceTableModel::columnCount(const QModelIndex & parent) const {
    return NCOLUMNS;
}

QVariant DeviceTableModel::data(const QModelIndex & index, int role) const {
    if (!index.isValid())
        return QVariant();

    if (index.row() >= _entries.size())
        return QVariant();

    if (role != Qt::DisplayRole)
        return QVariant();

    auto & item = _entries.at(index.row());
    switch (index.column()) {
    case 0:
        return QString::fromStdString(item.device());
    case 1:
        return QString::fromStdString(item.platform());
    case 2:
        return item.device_id();
    case 3:
        if (item.platform() == "OpenCL")
            return item.platform_id();
        else
            return QVariant();
    default:
        return QVariant();
    }
}


QVariant DeviceTableModel::headerData(int section, Qt::Orientation orientation, int role) const {
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
            return QString("Device ID");
        case 3:
            return QString("Platform ID");
        default:
            return QVariant();
        }
    } else {
        return QString("%1").arg(section);
    }
}

#include "DeviceTableModel.moc"