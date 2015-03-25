#include "CentralWidget.h"

#include <QGridLayout>
#include <QVBoxLayout>

CentralWidget::CentralWidget(): QWidget()
{
    QGridLayout * grid = new QGridLayout();
    grid->addWidget(createPlatformsPicker(), 0, 0);
    grid->addWidget(createOptions(), 1, 0);
    grid->addWidget(createAvailableDevices(), 0, 1, 2, 1);
    grid->addWidget(createResults(), 2, 0, 1, 2);
    setLayout(grid);
}


QGroupBox * CentralWidget::createPlatformsPicker() {
    openCLButton_ = new QRadioButton(tr("OpenCL - F@H"));
    cudaButton_ = new QRadioButton(tr("CUDA - OpenMM"));
    connect(openCLButton_, SIGNAL(clicked()), this, SLOT(updateOpenCLDevices()));
    connect(cudaButton_, SIGNAL(clicked()), this, SLOT(updateCUDADevices()));
    openCLButton_->setChecked(true);
    QVBoxLayout *vbox = new QVBoxLayout;
    vbox->addWidget(openCLButton_);
    vbox->addWidget(cudaButton_);
    vbox->addStretch(1);
    QGroupBox *groupBox = new QGroupBox(tr("Platform"));
    groupBox->setLayout(vbox);
    return groupBox;
}

QGroupBox * CentralWidget::createOptions() {
    explicitBox_ = new QCheckBox(tr("Explicit"));
    explicitBox_->setChecked(true);
    implicitBox_ = new QCheckBox(tr("Implicit"));
    implicitBox_->setChecked(true);
    precisionBox_ = new QCheckBox(tr("Double Precision"));
    verifyAccuracyBox_ = new QCheckBox(tr("Verify Accuracy"));
    verifyAccuracyBox_->setChecked(false);
    stressTestBox_ = new QCheckBox(tr("Stress Test"));
    stressTestBox_->setChecked(false);

    QVBoxLayout *vbox = new QVBoxLayout;
    vbox->addWidget(explicitBox_);
    vbox->addWidget(implicitBox_);
    vbox->addWidget(verifyAccuracyBox_);
    vbox->addWidget(precisionBox_);
    vbox->addWidget(stressTestBox_);
    vbox->addStretch(1);
    QGroupBox *groupBox = new QGroupBox(tr("Options"));
    groupBox->setLayout(vbox);
    return groupBox;
}
void CentralWidget::updateCUDADevices() {
    qLWidgetHandle_->clear();
    try {
        cudaDeviceMap_ = GPUInfo::getCUDADevices();
        if(cudaDeviceMap_.size() == 0) {
            qLWidgetHandle_->addItem("No CUDA devices found.");
            qLWidgetHandle_->setSelectionMode(QAbstractItemView::NoSelection);
            return;
        }
        for(const auto & it : cudaDeviceMap_) {
            qLWidgetHandle_->addItem(it.first.c_str());
        }
        qLWidgetHandle_->setSelectionMode(QAbstractItemView::ExtendedSelection);
        qLWidgetHandle_->setCurrentRow(0);
    } catch(const std::exception &e) {
        qLWidgetHandle_->addItem(e.what());
        qLWidgetHandle_->setCurrentRow(0);
        qLWidgetHandle_->setSelectionMode(QAbstractItemView::NoSelection);
    }
}

void CentralWidget::updateOpenCLDevices() {
    openCLDeviceMap_ = GPUInfo::getOpenCLDevices();
    qLWidgetHandle_->clear();
    for(auto const & it : openCLDeviceMap_) {
        qLWidgetHandle_->addItem(it.first.c_str());
    }
    qLWidgetHandle_->setSelectionMode(QAbstractItemView::ExtendedSelection);
    qLWidgetHandle_->setCurrentRow(0);
}


QGroupBox * CentralWidget::createAvailableDevices() {
    QGroupBox *groupBox = new QGroupBox(tr("Available Devices"));
    qLWidgetHandle_ = new QListWidget;
    updateOpenCLDevices();
    QVBoxLayout *vbox = new QVBoxLayout;
    vbox->addWidget(qLWidgetHandle_);
    vbox->addStretch(1);
    groupBox->setLayout(vbox);
    return groupBox;
}

QGroupBox *CentralWidget::createResults() {
    QLabel *explicitLabel = new QLabel("Explicit Solvent: ");
    QLabel *implicitLabel = new QLabel("Implicit Solvent: ");
    QGridLayout *gLayout = new QGridLayout(this);
    gLayout->addWidget(explicitLabel,0,0);
    gLayout->addWidget(implicitLabel,1,0);

    explicitResult_ = new QLineEdit();
    explicitResult_->setReadOnly(true);
    implicitResult_ = new QLineEdit();
    implicitResult_->setReadOnly(true);

    gLayout->addWidget(explicitResult_,0,1);
    gLayout->addWidget(implicitResult_,1,1);

    startButton_ = new OneShotButton(tr("&Start"), this);
    connect(startButton_, SIGNAL(clicked()), this, SLOT(setupSimulation()));
    startButton_->setMinimumWidth(50);
    startButton_->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    gLayout->addWidget(startButton_, 0,2,2,1);

    gLayout->setColumnStretch(0, 15);
    gLayout->setColumnStretch(1, 85);
    gLayout->setColumnStretch(2, 10);

    QGroupBox *groupBox = new QGroupBox(tr("&Results"));
    groupBox->setLayout(gLayout);
    return groupBox;
}


#include "CentralWidget.moc"
