#ifndef WORK_UNIT_TABLE_MODEL_H
#define WORK_UNIT_TABLE_MODEL_H

#include <vector>

#include <QAbstractTableModel>

#include "../WorkUnit.h"

enum WUTABLE_COLS {
    CodeName = 0,
    FullName = 1,
    Description = 2,
    StepChunk = 3,
};

const static int WUTABLE_NCOLS = 4;

class WorkUnitTableModel  : public QAbstractTableModel {

private:
    std::vector<WorkUnit> _entries;

public:
    WorkUnitTableModel();
    int rowCount(const QModelIndex & parent) const;
    int columnCount(const QModelIndex & parent) const;
    QVariant data(const QModelIndex & index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
};


#endif //WORK_UNIT_TABLE_MODEL_H
