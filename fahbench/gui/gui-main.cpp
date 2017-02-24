#include <QApplication>
#include <QString>
#include <QIcon>
#include <QStyleFactory>

#include "FAHBenchVersion.h"
#include "MainWindow.h"

int main(int argc, char * argv[]) {
    QApplication app(argc, argv);
    app.setOrganizationName("Folding@Home");
    app.setApplicationName("FAHBench");
    app.setWindowIcon(QIcon(":FahIcon.ico"));
#ifdef _WIN32
    app.setStyle(QStyleFactory::create("windows"));
#else
    app.setStyle(QStyleFactory::create("fusion"));
#endif

    app.setApplicationVersion(QString::fromStdString(getVersion()));
    MainWindow main_win;
    main_win.show();
    return app.exec();
}
