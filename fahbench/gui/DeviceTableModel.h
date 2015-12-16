#ifndef DEVICE_TABLE_MODEL_H_
#define DEVICE_TABLE_MODEL_H_

#include <vector>

#include <QAbstractTableModel>

#include "../GPUInfo.h"
#include "../Device.h"


class DeviceTableModel : public QAbstractTableModel {
    Q_OBJECT

private:
    std::vector<Device> _entries;
    std::vector<std::runtime_error> _errors;

public:
    DeviceTableModel();
    int rowCount(const QModelIndex & parent) const;
    int columnCount(const QModelIndex & parent) const;
    QVariant data(const QModelIndex & index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;

    const std::vector<Device> & entries() const;
    const std::vector<std::runtime_error> & errors() const;


};

#endif // DEVICE_TABLE_MODEL_H_
