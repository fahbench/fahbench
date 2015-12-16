//
// Created by harrigan on 12/16/15.
//

#ifndef RESULTS_WIDGET_H
#define RESULTS_WIDGET_H

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>


class ResultsWidget : public QWidget {
    Q_OBJECT

private:
    QVBoxLayout  * layout;

    QLabel * results_label;

public:
    ResultsWidget();


    void set_results();




};


#endif //RESULTS_WIDGET_H
