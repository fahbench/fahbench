#include <QApplication>
#include <QString>
#include <QIcon>

#include "FAHBenchVersion.h"
#include "MainWindow.h"

int main(int argc, char * argv[]) {
    QApplication app(argc, argv);
    app.setOrganizationName("Folding@Home");
    app.setApplicationName("FAHBench");
    app.setWindowIcon(QIcon(":FahIcon.ico"));

    app.setApplicationVersion(QString::fromStdString(getVersion()));
    MainWindow main_win;
    main_win.show();
    return app.exec();
}
