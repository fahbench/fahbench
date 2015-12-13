#include "CentralWidget.h"
#include "../FAHBenchVersion.h"

#include <QMessageBox>

QSize CentralWidget::sizeHint() const {
    return QSize(850, 450);
}


CentralWidget::CentralWidget() : QWidget() {

    try {
        device_table_model = new DeviceTableModel();
    } catch (const std::runtime_error & e) {
        QMessageBox::warning(this, "Device error", e.what());
    }
    device_table_view = new QTableView();

    layout_vbox = new QVBoxLayout;
    layout_leftright = new QHBoxLayout;

    layout_form = new QFormLayout;
    device_wid = new QComboBox;
    device_wid->setModel(device_table_model);
    device_wid->setView(device_table_view);
    layout_form->addRow("Device", device_wid);
    precision_wid = new QComboBox;
    precision_wid->addItem("single");
    precision_wid->addItem("double");
    layout_form->addRow("Precision", precision_wid);
    wu_wid = new QComboBox;
    wu_wid->addItem("dhfr");
    layout_form->addRow("WU", wu_wid);
    accuracy_check_wid = new QCheckBox("Enabled");
    layout_form->addRow("Accuracy Check", accuracy_check_wid);
    nan_check_wid = new QSpinBox;
    nan_check_wid->setValue(0);
    nan_check_wid->setMinimum(0);
    nan_check_wid->setSpecialValueText("0 - Disabled");
    nan_check_wid->setSingleStep(10);
    nan_check_wid->setSuffix(" steps");
    layout_form->addRow("NaN Check", nan_check_wid);
    run_length_wid = new QSpinBox;
    run_length_wid->setValue(60);
    run_length_wid ->setMinimum(1);
    run_length_wid -> setSingleStep(10);
    run_length_wid->setSuffix(" s");
    layout_form->addRow("Run length", run_length_wid);
    layout_leftright->addLayout(layout_form);

    layout_rightpanel = new QVBoxLayout;
    placeholder_wid = new QLabel("Placeholder");
    layout_rightpanel->addWidget(placeholder_wid);

    layout_leftright->addLayout(layout_rightpanel);

    layout_vbox->addLayout(layout_leftright);

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


void CentralWidget::progress_update(int i, int numSteps, float score) {
    progress_bar->setMaximum(numSteps);
    progress_bar->setValue(i);
    status_bar->setText(QString("Current score: %1").arg(score));
}

void CentralWidget::message_update(const QString & s) {
    status_bar->setText(s);
}


#include "CentralWidget.moc"
