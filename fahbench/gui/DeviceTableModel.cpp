#include "DeviceTableModel.h"
#include "CentralWidget.h"
#include <exception>

const static int NCOLUMNS = 7;

DeviceTableModel::DeviceTableModel(CentralWidget & parent) :
    _entries() {
    try {
        auto opencl_devices = GPUInfo::getOpenCLDevices();
        _entries.insert(_entries.end(), opencl_devices.begin(), opencl_devices.end());
    } catch (const std::exception & err) {
        parent.message_update(err.what());
    }
    try {
        auto cuda_devices = GPUInfo::getCUDADevices();
        _entries.insert(_entries.end(), cuda_devices.begin(), cuda_devices.end());
    } catch (const std::exception & err) {
        parent.message_update(err.what());
    }

}

const std::vector< Device > & DeviceTableModel::entries() const {
    const std::vector<Device> & ret = _entries;
    return ret;
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

    if (index.row() < 0 || ((unsigned)index.row() >= _entries.size()))
        return QVariant();

    if (role == Qt::DisplayRole) {
        auto & item = _entries.at(index.row());
        switch (index.column()) {
        case 0:
            return QString("Add");
        case 1:
            return QString::fromStdString(item.device());
        case 2:
            return QString::fromStdString(item.platform());
        case 3:
            return item.device_id();
        case 4:
            if (item.platform() == "OpenCL")
                return item.platform_id();
            else
                return QVariant();
        case 5:
            return QString::fromStdString(item.platform_version);
        case 6:
            return QString::fromStdString(item.device_version);
        default:
            return QVariant();
        }
    } else if (role == Qt::ToolTipRole) {
        switch (index.column()) {
        case 0:
            return QString("Queue this device");
        default:
            return QVariant();
        }
    } else {
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
            return QString("");
        case 1:
            return QString("Device");
        case 2:
            return QString("Platform");
        case 3:
            return QString("Device ID");
        case 4:
            return QString("Platform ID");
        case 5:
            return QString("Platform version");
        case 6:
            return QString("Device version");
        default:
            return QVariant();
        }
    } else {
        return QString("%1").arg(section);
    }
}

#include <QDebug> // TODO

void DeviceTableModel::device_clicked(const QModelIndex & index) {
    if (index.column() == 0) {
        qDebug() << QString::fromStdString(entries()[index.row()].device());
    }
}


#include "DeviceTableModel.moc"