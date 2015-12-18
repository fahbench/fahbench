#include "CentralWidget.h"
#include "../FAHBenchVersion.h"

#include <QGridLayout>
#include <QVBoxLayout>

QSize CentralWidget::sizeHint() const {
    return QSize(850, 450);
}


CentralWidget::CentralWidget() : QWidget() {
    layout_a = new QVBoxLayout();

    // Make sure this comes before construction of tables
    status_bar = new QLabel(QString("FAHBench v%1").arg(QString::fromStdString(getVersion())));

    // Tables
    layout_a1 = new QHBoxLayout();
    make_device_table();
    make_simulation_table();
    layout_a1->addLayout(device_vbox);
    layout_a1->addLayout(simulation_vbox, 1);
    layout_a->addLayout(layout_a1);

    // Status bar
    layout_a->addWidget(status_bar);

    // Bottom panel
    layout_a2 = new QHBoxLayout();
    progress_bar = new QProgressBar();
    layout_a2->addWidget(progress_bar);
    start_button = new QPushButton("Start");
    layout_a2->addWidget(start_button);
    layout_a->addLayout(layout_a2);

    setLayout(layout_a);
}

void CentralWidget::make_device_table() {
    device_table_view = new DeviceTableView();
    device_table_model = new DeviceTableModel(*this);
    device_table_view->setModel(device_table_model);
    connect(device_table_view, &DeviceTableView::clicked, device_table_model, &DeviceTableModel::device_clicked);

    device_vbox = new QVBoxLayout();
    device_vbox->addWidget(new QLabel("Devices"));
    device_vbox->addWidget(device_table_view);
}

void CentralWidget::make_simulation_table() {
    simulation_table_view = new SimulationTableView();
    simulation_table_model = new SimulationTableModel(device_table_model);
    simulation_table_view->setModel(simulation_table_model);
    connect(simulation_table_view, &SimulationTableView::clicked, simulation_table_model, &SimulationTableModel::simulation_clicked);

    simulation_vbox = new QVBoxLayout();
    simulation_vbox->addWidget(new QLabel("Benchmarking runs"));
    simulation_vbox->addWidget(simulation_table_view);
}


void CentralWidget::progress_update(int i, int numSteps, float score) {
    progress_bar->setMaximum(numSteps);
    progress_bar->setValue(i);
    status_bar->setText(QString("Current score: %1").arg(score));
}

void CentralWidget::message_update(const QString & s) {
    status_bar->setText(s);
}



#include "CentralWidget.moc"
