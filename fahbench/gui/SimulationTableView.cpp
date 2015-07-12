#include "SimulationTableView.h"

#include <QStyleOptionButton>
#include <QApplication>
#include <QComboBox>

#include <QDebug> // TODO remove

QWidget * SimulationTableItemDelegate::createEditor(QWidget * parent, const QStyleOptionViewItem & option, const QModelIndex & index) const {

    if (index.column() == 2) {
        QStringList values;
        values << "One" << "Two";

        auto cb = new QComboBox(parent);
        cb->addItems(values);
        cb->setGeometry(option.rect);
        qDebug() << "createEditor";
        return cb;
    }

    return QStyledItemDelegate::createEditor(parent, option, index);
}



void SimulationTableView::setModel(QAbstractItemModel * model) {
    QTableView::setModel(model);
    resizeColumnsToContents();
    resizeRowsToContents();
    setEditTriggers(QAbstractItemView::AllEditTriggers);
    setItemDelegate(new SimulationTableItemDelegate);
}

#include "SimulationTableView.moc"
