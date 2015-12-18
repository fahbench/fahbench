#ifndef DEVICE_TABLE_MODEL_H_
#define DEVICE_TABLE_MODEL_H_

#include <vector>

#include <QString>
#include <QAbstractTableModel>

#include "../GPUInfo.h"
#include "../Device.h"

class CentralWidget; // resolve circular include

class DeviceTableModel : public QAbstractTableModel {

private:
    std::vector<Device> _entries;

public:
    DeviceTableModel(CentralWidget & parent);
    int rowCount(const QModelIndex & parent) const;
    int columnCount(const QModelIndex & parent) const;
    QVariant data(const QModelIndex & index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;

    const std::vector< Device > & entries() const;

public slots:
    void device_clicked(const QModelIndex & index);

};

#endif // DEVICE_TABLE_MODEL_H_
