#include <algorithm>
#include "WorkUnitTableModel.h"

bool put_dhfr_first(const WorkUnit & a, const WorkUnit & b) {
    return a.codename() == "dhfr";
}


WorkUnitTableModel::WorkUnitTableModel()
    : _entries(WorkUnit::available_wus()) {
    std::sort(_entries.begin(), _entries.end(), put_dhfr_first);

}

int WorkUnitTableModel::rowCount(const QModelIndex & parent) const {
    return _entries.size();
}

int WorkUnitTableModel::columnCount(const QModelIndex & parent) const {
    return WUTABLE_NCOLS;
}

QVariant WorkUnitTableModel::data(const QModelIndex & index, int role) const {
    if (!index.isValid())
        return QVariant();

    if (index.row() < 0 || ((unsigned) index.row() >= _entries.size()))
        return QVariant();

    if (role != Qt::DisplayRole)
        return QVariant();

    auto & item = _entries.at(index.row());
    switch (index.column()) {
    case WUTABLE_COLS::CodeName:
        return QString::fromStdString(item.codename());
    case WUTABLE_COLS::FullName:
        return QString::fromStdString(item.fullname());
    case WUTABLE_COLS::Description:
        return QString::fromStdString(item.description());
    case WUTABLE_COLS::StepChunk:
        return item.step_chunk();
    default:
        return QVariant();
    }
}

QVariant WorkUnitTableModel::headerData(int section, Qt::Orientation orientation, int role) const {
    if (role != Qt::DisplayRole) {
        return QVariant();
    }

    if (orientation == Qt::Horizontal) {
        switch (section) {
        case WUTABLE_COLS::CodeName:
            return QString("Code");
        case WUTABLE_COLS::FullName:
            return QString("Full Name");
        case WUTABLE_COLS::Description:
            return QString("Description");
        case WUTABLE_COLS::StepChunk:
            return QString("Steps");
        default:
            return QVariant();
        }
    } else {
        return QString("%1").arg(section);
    }
}

#include "WorkUnitTableModel.moc"