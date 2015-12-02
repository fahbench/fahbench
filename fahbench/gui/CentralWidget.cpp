#include "CentralWidget.h"
#include "../FAHBenchVersion.h"


QSize CentralWidget::sizeHint() const {
    return QSize(850, 450);
}


CentralWidget::CentralWidget() : QWidget() {
    layout_vbox = new QVBoxLayout(this);
    layout_form = new QFormLayout(this);

    device_wid = new QComboBox(this);
    device_wid->addItem("Device 1");
    layout_form->addRow("Device", device_wid);
    platform_wid = new QComboBox(this);
    platform_wid->addItem("OpenCL");
    layout_form->addRow("Platform", platform_wid);
    precision_wid = new QComboBox(this);
    precision_wid->addItem("single");
    precision_wid->addItem("double");
    layout_form->addRow("Precision", precision_wid);
    wu_wid = new QComboBox(this);
    wu_wid->addItem("dhfr");
    layout_form->addRow("WU", wu_wid);
    //TODO Advanced wu config with xmls
    accuracy_check_wid = new QCheckBox("Enabled", this);
    layout_form->addRow("Accuracy Check", accuracy_check_wid);
    nan_check_wid = new QSpinBox(this);
    layout_form->addRow("NaN Check", nan_check_wid);
    run_length_wid = new QSpinBox(this);
    layout_form->addRow("Run length (s)", run_length_wid);
    step_chunk_wid = new QSpinBox(this);
    layout_form->addRow("Step chunk", step_chunk_wid);
    layout_vbox->addLayout(layout_form);

    // Make sure this comes before construction of tables
    status_bar = new QLabel(QString("FAHBench v%1").arg(QString::fromStdString(getVersion())));
    layout_vbox->addWidget(status_bar);


    // Bottom panel
    layout_bot = new QHBoxLayout();
    progress_bar = new QProgressBar();
    layout_bot->addWidget(progress_bar);
    start_button = new QPushButton("Start");
    layout_bot->addWidget(start_button);
    layout_vbox->addLayout(layout_bot);

    setLayout(layout_vbox);
}

void CentralWidget::make_device_table() {
    device_table_view = new QTableView();
    device_table_model = new DeviceTableModel(*this);
    device_table_view->setModel(device_table_model);

    auto tv = device_table_view;
    tv->setSelectionBehavior(QAbstractItemView::SelectRows);
    tv->setShowGrid(false);
    tv->resizeColumnsToContents();
    tv->resizeRowsToContents();

}

void CentralWidget::make_simulation_table() {
    simulation_table_view = new QTableView();
    simulation_table_model = new SimulationTableModel(device_table_model);
    simulation_table_view->setModel(simulation_table_model);
    simulation_table_view->resizeColumnsToContents();
    simulation_table_view->resizeRowsToContents();

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
