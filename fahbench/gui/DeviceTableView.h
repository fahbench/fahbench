#ifndef DEVICETABLEVIEW_H
#define DEVICETABLEVIEW_H

#include <QTableView>
#include <QStyledItemDelegate>

class DeviceTableItemDelegate : public QStyledItemDelegate {
    Q_OBJECT

public:
    void paint(QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index) const;
};

class DeviceTableView : public QTableView {
    Q_OBJECT

public:
    void setModel(QAbstractItemModel * model);

};

#endif // DEVICETABLEVIEW_H
