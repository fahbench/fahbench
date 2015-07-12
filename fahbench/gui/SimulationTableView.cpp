#include "SimulationTableView.h"

#include <QStyleOptionButton>
#include <QApplication>
#include <QMouseEvent>


void SimulationTableView::setModel(QAbstractItemModel * model) {
    QTableView::setModel(model);
    resizeColumnsToContents();
    resizeRowsToContents();
}

#include "SimulationTableView.moc"
