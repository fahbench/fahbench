#include <OpenMM.h>

#include <QMessageBox>
#include <QDebug>
#include <QHeaderView>

#include "CentralWidget.h"
#include "FAHBenchVersion.h"

QSize CentralWidget::sizeHint() const {
    return QSize(650, 350);
}

inline std::string getOpenMMVersion() {
    return OpenMM::Platform::getOpenMMVersion();
}


CentralWidget::CentralWidget() : QWidget() {

    device_table_model = new DeviceTableModel;
    for (const auto & e : device_table_model->errors()) {
        QString msg;
        msg += "There was a problem getting compatible GPU devices. ";
        msg += "The following error code may be helpful:\n";
        msg += e.what();
        QMessageBox::warning(this, "Device error", msg);
    }
    device_table_view = new QTableView;

    wu_table_model = new WorkUnitTableModel;
    wu_table_view = new QTableView;

    layout_vbox = new QVBoxLayout;
    layout_leftright = new QHBoxLayout;

    layout_form = new QFormLayout;
    device_wid = new QComboBox;
    device_wid->setModel(device_table_model);
	device_table_view->setMinimumWidth(600);
    device_wid->setView(device_table_view);
    layout_form->addRow("Device", device_wid);
    openmm_platform_wid = new QComboBox;
    openmm_platform_wid->addItem("OpenCL");
    openmm_platform_wid->addItem("CPU");
    connect(openmm_platform_wid, SIGNAL(currentIndexChanged(int)), this, SLOT(openmm_platform_changed(int)));
    layout_form->addRow("Compute", openmm_platform_wid);
    precision_wid = new QComboBox;
    precision_wid->addItem("mixed");
    precision_wid->addItem("single");
    precision_wid->addItem("double");
    layout_form->addRow("Precision", precision_wid);
    wu_wid = new QComboBox;
    wu_wid->setModel(wu_table_model);
    wu_wid->setView(wu_table_view);
	wu_table_view->verticalHeader()->setVisible(false);
	wu_table_view->horizontalHeader()->setStretchLastSection(true);
	wu_table_view->hideColumn(WUTABLE_COLS::Description); 
	wu_table_view->hideColumn(WUTABLE_COLS::StepChunk);
    layout_form->addRow("WU", wu_wid);
	wudesc_wid = new QLabel;
	wu_select_changed(0);
	connect(wu_wid, static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, &CentralWidget::wu_select_changed);
	layout_form->addRow("WU Description", wudesc_wid);
    accuracy_check_wid = new QCheckBox("Enabled");
    accuracy_check_wid->setChecked(true);
    layout_form->addRow("Accuracy Check", accuracy_check_wid);
    nan_check_wid = new QSpinBox;
    nan_check_wid->setValue(0);
    nan_check_wid->setMinimum(0);
	nan_check_wid->setMaximum(10'000);
    nan_check_wid->setSpecialValueText("0 - Disabled");
    nan_check_wid->setSingleStep(10);
    nan_check_wid->setSuffix(" steps");
    layout_form->addRow("NaN Check", nan_check_wid);
    run_length_wid = new QSpinBox;
    run_length_wid->setValue(1);
    run_length_wid->setMinimum(1);
    run_length_wid->setMaximum(60 * 24);
    run_length_wid->setSingleStep(1);
    run_length_wid->setSuffix(" mins");
	run_length_wid->setSpecialValueText("1 min");
    layout_form->addRow("Run length", run_length_wid);
    layout_leftright->addLayout(layout_form);
    layout_leftright->setStretchFactor(layout_form, Qt::MaximumSize);

    results_wid = new ResultsWidget;
    layout_leftright->addWidget(results_wid);

    layout_vbox->addLayout(layout_leftright);

    status_bar = new QLabel(QString("FAHBench %1 with OpenMM %2")
                                    .arg(QString::fromStdString(getVersion()))
                                    .arg(QString::fromStdString(getOpenMMVersion())));
    layout_vbox->addWidget(status_bar);


    // Bottom panel
    layout_bot = new QHBoxLayout();
    progress_bar = new ProgressBar();
    layout_bot->addWidget(progress_bar->widget());
    start_button = new QPushButton("Start");
    layout_bot->addWidget(start_button);
    cancel_button = new QPushButton("Cancel");
    cancel_button->setEnabled(false);
    layout_bot->addWidget(cancel_button);
    layout_vbox->addLayout(layout_bot);

    setLayout(layout_vbox);

    device_table_view->setSelectionBehavior(QAbstractItemView::SelectRows);
    device_table_view->resizeColumnsToContents();
    device_table_view->resizeRowsToContents();

    wu_table_view->setSelectionBehavior(QAbstractItemView::SelectRows);
    wu_table_view->resizeColumnsToContents();
    wu_table_view->resizeRowsToContents();
}

void CentralWidget::configure_simulation(Simulation & sim) const {
    switch (openmm_platform_wid->currentIndex()) {
        case OPENMM_PLATFORMS::OpenCL: {
            sim.platform = "OpenCL";
            auto sel_device = device_table_model->entries().at(device_wid->currentIndex());
            sim.deviceId = sel_device.device_id();
            sim.platformId = sel_device.platform_id();
        }
            break;
        case OPENMM_PLATFORMS::CPU:
            sim.platform = "CPU";
            break;
        default:
            throw std::runtime_error("Unknown OpenMM Platform");
    }

    // Maybe seperate display text from actual text passed to OpenMM?
    sim.precision = precision_wid->currentText().toStdString();

    WorkUnit wu(wu_wid->currentText().toStdString());
    sim.work_unit = wu;

    sim.verifyAccuracy = accuracy_check_wid->isChecked();
    sim.nan_check_freq = nan_check_wid->value();

    sim.run_length = std::chrono::minutes(run_length_wid->value());

    qDebug() << QString::fromStdString(sim.summary());
}


void CentralWidget::progress_update(int i, int numSteps, float score) {
    progress_bar->setMaximum(numSteps);
    progress_bar->setValue(i);
    status_bar->setText(QString("Current score: %1").arg(score));
}

void CentralWidget::message_update(const QString & s) {
    status_bar->setText(s);
}

void CentralWidget::openmm_platform_changed(int index) {
    qDebug() << "OpenMM platform changed";
    switch (index) {
        //TODO: Enum
        case OPENMM_PLATFORMS::OpenCL:
            device_wid->setEnabled(true);
            break;
        case OPENMM_PLATFORMS::CPU:
            device_wid->setEnabled(false);
            break;
        default:
            throw std::runtime_error("Unknown openmm platform selection");
    }

}

void CentralWidget::wu_select_changed(int index) {
	qDebug() << "WU Selection changed";
	if (index < 0)
		return;
	auto desc_index = wu_table_model->index(index, WUTABLE_COLS::Description);
	auto desc = wu_table_model->data(desc_index, Qt::DisplayRole);
	wudesc_wid->setText(desc.toString());
}


#include "CentralWidget.moc"
