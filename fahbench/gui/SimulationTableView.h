#ifndef SIMULATIONTABLEVIEW_H
#define SIMULATIONTABLEVIEW_H

#include <QTableView>
#include <QStyledItemDelegate>



class SimulationTableView : public QTableView {
    Q_OBJECT

public:
    void setModel(QAbstractItemModel * model);


};

#endif // SIMULATIONTABLEVIEW_H
