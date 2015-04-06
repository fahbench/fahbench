#ifndef CENTRALWIDGET_H
#define CENTRALWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QTableView>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QProgressBar>
#include <QPushButton>

#include "SimulationTableModel.h"
#include "DeviceTableModel.h"


class CentralWidget :  public QWidget
{
    Q_OBJECT

public:
    CentralWidget();

private:
	
	QVBoxLayout * layout_a;
	QHBoxLayout * layout_a1;
	QHBoxLayout * layout_a2;

	DeviceTableModel * device_table_model;
	QTableView * device_table_view;
	SimulationTableModel * simulation_table_model;
	QTableView * simulation_table_view;

	QLabel * status_bar;

	QProgressBar * progress_bar;
	QPushButton * start_button;


};

#endif // CENTRALWIDGET_H
