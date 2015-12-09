#ifndef DEVICE_TABLE_MODEL_H_
#define DEVICE_TABLE_MODEL_H_

#include <vector>

#include <QAbstractTableModel>

#include "../GPUInfo.h"
#include "../Device.h"


class DeviceTableModel : public QAbstractTableModel {

private:
    std::vector<Device> _entries;

public:
    DeviceTableModel();
    int rowCount(const QModelIndex & parent) const;
    int columnCount(const QModelIndex & parent) const;
    QVariant data(const QModelIndex & index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;

    const std::vector<Device> & entries() const;

};

#endif // DEVICE_TABLE_MODEL_H_
