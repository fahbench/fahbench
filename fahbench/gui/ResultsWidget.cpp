//
// Created by harrigan on 12/16/15.
//

#include "ResultsWidget.h"

ResultsWidget::ResultsWidget() {
    layout = new QVBoxLayout;

    results_label = new QLabel(
        "<h1>FAHBench Results</h1><br />"
        "<em>Click start to benchmark your machine</em>"
    );

    layout->addWidget(results_label);
    layout->addStretch();
    setLayout(layout);

}

#include "ResultsWidget.moc"