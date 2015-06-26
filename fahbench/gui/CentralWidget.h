#ifndef CENTRALWIDGET_H
#define CENTRALWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QProgressBar>
#include <QPushButton>

#include "SimulationTableModel.h"
#include "DeviceTableModel.h"
#include "DeviceTableView.h"


class CentralWidget :  public QWidget {
    Q_OBJECT

public:
    CentralWidget();
    QSize sizeHint() const;

    DeviceTableModel * device_table_model;
    QTableView * device_table_view;
    SimulationTableModel * simulation_table_model;
    QTableView * simulation_table_view;

    QLabel * status_bar;

    QProgressBar * progress_bar;
    QPushButton * start_button;

private:
    QVBoxLayout * layout_a;
    QHBoxLayout * layout_a1;
    QHBoxLayout * layout_a2;

    QVBoxLayout * device_vbox;
    QVBoxLayout * simulation_vbox;

    // Helper functions
    void make_device_table();
    void make_simulation_table();

public slots:
    void progress_update(const int &, const int &, const double &);
    void message_update(const QString &);

};

#endif // CENTRALWIDGET_H
