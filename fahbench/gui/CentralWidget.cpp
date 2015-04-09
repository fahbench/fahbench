#include "CentralWidget.h"

#include <QGridLayout>
#include <QVBoxLayout>

QSize CentralWidget::sizeHint() const {
    return QSize(850, 450);
}


CentralWidget::CentralWidget() : QWidget() {
    layout_a = new QVBoxLayout();

    layout_a1 = new QHBoxLayout();
    make_device_table();
    layout_a1->addWidget(device_table_view);
    simulation_table_view = new QTableView();
    simulation_table_model = new SimulationTableModel();
    simulation_table_view->setModel(simulation_table_model);
    layout_a1->addWidget(simulation_table_view, 1);
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

void CentralWidget::make_device_table() {
    device_table_view = new QTableView();
    device_table_model = new DeviceTableModel();
    device_table_view->setModel(device_table_model);

    auto tv = device_table_view;
    tv->setSelectionBehavior(QAbstractItemView::SelectRows);
    tv->setShowGrid(false);
    tv->resizeColumnsToContents();
    tv->resizeRowsToContents();
}

void CentralWidget::progress_update(const int & i, const int & numSteps, const double & score) {
    progress_bar->setMaximum(numSteps);
    progress_bar->setValue(i);
    status_bar->setText(QString("Current score: %1").arg(score));
}

void CentralWidget::message_update(const QString & s) {
    status_bar->setText(s);
}



#include "CentralWidget.moc"
