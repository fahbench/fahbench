#ifndef CENTRALWIDGET_H
#define CENTRALWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QListWidget>
#include <QGroupBox>
#include <QCheckBox>
#include <QRadioButton>
#include <QLineEdit>

#include "../GPUInfo.h"
#include "OneShotButton.h"


class CentralWidget :  public QWidget
{
    Q_OBJECT

public:
    CentralWidget();

    // Options
    QCheckBox *explicitBox_;
    QCheckBox *implicitBox_;
    QCheckBox *precisionBox_;
    QCheckBox *verifyAccuracyBox_;
    QCheckBox *stressTestBox_;

    // Device List
    QListWidget *qLWidgetHandle_;

    OneShotButton *startButton_;

    // Platform Buttons
    QRadioButton *cudaButton_;
    QRadioButton *openCLButton_;

    // Result Boxes
    QLineEdit *explicitResult_;
    QLineEdit *implicitResult_;
    
private slots:
    // updates the internal openCLDeviceMap_ object
    // by querying the OpenCL drivers at run-time
    // the available OpenCL devices are vendor specific
    void updateOpenCLDevices();

    // updates the internal cudaDeviceMaps_ object
    void updateCUDADevices();


private:
    QGroupBox *createPlatformsPicker();
    QGroupBox *createOptions();
    QGroupBox *createAvailableDevices();
    QGroupBox *createResults();

    std::map<std::string, OpenCLId> openCLDeviceMap_;
    std::map<std::string, int> cudaDeviceMap_;


};

#endif // CENTRALWIDGET_H
