#ifndef INFLOWAPPLICATION_H
#define INFLOWAPPLICATION_H

#include <QtGui/QApplication>
#include <QtCore/QPointer>
#include  "mainwindow.h"





class InflowApplication : public QApplication {
     Q_OBJECT

public:
     InflowApplication(int &argc, char **argv);
     MainWindow* newMainWindow();
private:
     QList<QPointer<MainWindow> > m_mainWindows;

};

#endif // INFLOWAPPLICATION_H
