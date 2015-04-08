#include "CentralWidget.h"

#include <QGridLayout>
#include <QVBoxLayout>

CentralWidget::CentralWidget() : QWidget() {
    layout_a = new QVBoxLayout();

    layout_a1 = new QHBoxLayout();
    device_table_view = new QTableView();
    device_table_model = new DeviceTableModel();
    device_table_view->setModel(device_table_model);
    device_table_view->setSelectionBehavior(QAbstractItemView::SelectRows);
    layout_a1->addWidget(device_table_view);
    simulation_table_view = new QTableView();
    simulation_table_model = new SimulationTableModel();
    simulation_table_view->setModel(simulation_table_model);
    layout_a1->addWidget(simulation_table_view);
    layout_a->addLayout(layout_a1);

    status_bar = new QLabel("Status bar placeholder text");
    layout_a->addWidget(status_bar);

    layout_a2 = new QHBoxLayout();
    progress_bar = new QProgressBar();
    layout_a2->addWidget(progress_bar);
    start_button = new QPushButton("Start");
    layout_a2->addWidget(start_button);
    layout_a->addLayout(layout_a2);

    setLayout(layout_a);
}



#include "CentralWidget.moc"
