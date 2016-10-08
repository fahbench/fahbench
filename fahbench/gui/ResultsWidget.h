#ifndef RESULTS_WIDGET_H
#define RESULTS_WIDGET_H

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>

#include "SimulationResult.h"


class ResultsWidget : public QWidget {
    Q_OBJECT

private:
    QVBoxLayout  * layout;
    QLabel * results_label;

    QString status_string(const ResultStatus status) const;

public:
    ResultsWidget();

    void set_result(const SimulationResult & result);




};


#endif //RESULTS_WIDGET_H
