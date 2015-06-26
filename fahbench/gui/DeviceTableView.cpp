#include "DeviceTableView.h"

#include <QStyleOptionButton>
#include <QApplication>
#include <QMouseEvent>

void DeviceTableItemDelegate::paint(QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index) const {
    QStyleOptionButton button_option;
    switch (index.column()) {
    case 0:
        button_option.rect = option.rect;
        button_option.text = "+";
        QApplication::style()->drawControl(QStyle::CE_PushButton, &button_option, painter);
        break;
    default:
        QStyledItemDelegate::paint(painter, option, index);
    }
}


void DeviceTableView::setModel(QAbstractItemModel * model) {
    QTableView::setModel(model);
    setSelectionBehavior(QAbstractItemView::SelectRows);
    setShowGrid(false);
    resizeColumnsToContents();
    resizeRowsToContents();
    setItemDelegate(new DeviceTableItemDelegate());
}



#include "DeviceTableView.moc"
