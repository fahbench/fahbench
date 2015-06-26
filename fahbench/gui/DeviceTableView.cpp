#include "DeviceTableView.h"

void DeviceTableItemDelegate::paint(QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index) const {
    QStyledItemDelegate::paint(painter, option, index);
}



void DeviceTableView::setModel(QAbstractItemModel * model) {
    QTableView::setModel(model);
    setSelectionBehavior(QAbstractItemView::SelectRows);
    setShowGrid(false);
    resizeColumnsToContents();
    resizeRowsToContents();

}


#include "DeviceTableView.moc"
