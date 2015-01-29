#include "inflowapplication.h"

InflowApplication::InflowApplication(int &argc, char **argv)
: QApplication(argc, argv) {
     QCoreApplication::setOrganizationName(QLatin1String("Cenditel"));
     QCoreApplication::setApplicationName(QLatin1String("Inflow"));
     QCoreApplication::setApplicationVersion(QLatin1String("0.1.Beta"));

}

MainWindow* InflowApplication::newMainWindow() {
    MainWindow *newwindow = new MainWindow();
    m_mainWindows.prepend(newwindow);
    newwindow->show();
    return newwindow;
}
