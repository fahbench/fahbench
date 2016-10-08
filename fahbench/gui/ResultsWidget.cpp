#include "ResultsWidget.h"

const static QString RESULTS_HEADER("<h1>FAHBench Results</h1>");

ResultsWidget::ResultsWidget() {
    layout = new QVBoxLayout;

    QString text(RESULTS_HEADER);
    text += "<em>Click start to benchmark your machine.</em>";
    results_label = new QLabel(text);

    layout->addWidget(results_label);
    layout->addStretch();
    setLayout(layout);
}

void ResultsWidget::set_result(const SimulationResult & result) {
    QString text(RESULTS_HEADER);
    QString row("<tr><td>%1</td><td>%2</td></tr>");
    text += "<table width='100%'>";
    switch (result.status()) {
    case ResultStatus::FINISHED:
        text += row.arg("Score").arg(result.score());
        text += row.arg("Scaled Score").arg(result.scaled_score());
        text += row.arg("Atoms").arg(result.n_atoms());
    case ResultStatus::PENDING:
    case ResultStatus::QUEUED:
    case ResultStatus::INPROGRESS:
    case ResultStatus::FAILED:
    case ResultStatus::CANCELLED:
        text += row.arg("Status").arg(status_string(result.status()));
    }
    text += "</table>";
    results_label->setText(text);

}

QString ResultsWidget::status_string(const ResultStatus status) const {
    switch (status) {
    case ResultStatus::PENDING:
        return "Pending";
    case ResultStatus::QUEUED:
        return "Queued";
    case ResultStatus::INPROGRESS:
        return "In Progress";
    case ResultStatus::FAILED:
        return "Failed";
    case ResultStatus::CANCELLED:
        return "Cancelled";
    case ResultStatus::FINISHED:
        return "Finished";
    }

    return "Unknown!";
}

#include "ResultsWidget.moc"