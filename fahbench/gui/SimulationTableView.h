#ifndef SIMULATIONTABLEVIEW_H
#define SIMULATIONTABLEVIEW_H

#include <QTableView>
#include <QStyledItemDelegate>

class SimulationTableItemDelegate : public QStyledItemDelegate {
    Q_OBJECT

public:
    QWidget * createEditor(QWidget * parent,
                           const QStyleOptionViewItem & option,
                           const QModelIndex & index) const;
};

class SimulationTableView : public QTableView {
    Q_OBJECT

public:
    void setModel(QAbstractItemModel * model);


};

#endif // SIMULATIONTABLEVIEW_H
