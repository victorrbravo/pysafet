/****************************************************************************
** Meta object code from reading C++ file 'mainwindow.h'
**
** Created: Mon Oct 19 11:50:36 2009
**      by: The Qt Meta Object Compiler version 59 (Qt 4.4.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "mainwindow.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mainwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.4.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_MainWindow[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
      28,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // slots: signature, parameters, type, tag, flags
      12,   11,   11,   11, 0x08,
      20,   11,   11,   11, 0x08,
      29,   11,   11,   11, 0x08,
      41,   11,   11,   11, 0x08,
      55,   11,   11,   11, 0x08,
      72,   11,   11,   11, 0x08,
      86,   11,   11,   11, 0x08,
     105,   11,   11,   11, 0x08,
     124,   11,   11,   11, 0x08,
     141,   11,   11,   11, 0x08,
     150,   11,   11,   11, 0x08,
     169,   11,   11,   11, 0x08,
     188,   11,   11,   11, 0x08,
     205,   11,   11,   11, 0x08,
     225,   11,   11,   11, 0x08,
     262,  260,   11,   11, 0x08,
     306,   11,   11,   11, 0x28,
     328,  260,   11,   11, 0x08,
     373,   11,   11,   11, 0x28,
     410,  396,   11,   11, 0x08,
     462,  453,   11,   11, 0x28,
     484,   11,   11,   11, 0x08,
     499,  495,   11,   11, 0x0a,
     523,  495,   11,   11, 0x0a,
     551,  549,   11,   11, 0x0a,
     575,   11,   11,   11, 0x2a,
     595,   11,   11,   11, 0x0a,
     612,  609,   11,   11, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_MainWindow[] = {
    "MainWindow\0\0about()\0toSend()\0toLoadWeb()\0"
    "toInputForm()\0toInputConsole()\0"
    "toInputSign()\0toInputConfigure()\0"
    "toDelAllWorkflow()\0selInputTab(int)\0"
    "doQuit()\0doGeneralOptions()\0"
    "doWidgetsOptions()\0setToInputForm()\0"
    "setToInputConsole()\0"
    "setToInputManagementSignDocument()\0a\0"
    "showSmartMenuWidget(DockSbMenu::ShowAction)\0"
    "showSmartMenuWidget()\0"
    "showShowResultWidget(DockSbMenu::ShowAction)\0"
    "showShowResultWidget()\0filename,item\0"
    "drawWorkflow(QString,GraphicsSvgItemLive*)\0"
    "filename\0drawWorkflow(QString)\0"
    "toSbMenu()\0url\0linkClickedSbMenu(QUrl)\0"
    "linkClickedSbResult(QUrl)\0e\0"
    "setEnabledToolBar(bool)\0setEnabledToolBar()\0"
    "goPrincipal()\0ok\0executeJSCodeAfterLoad(bool)\0"
};

const QMetaObject MainWindow::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_MainWindow,
      qt_meta_data_MainWindow, 0 }
};

const QMetaObject *MainWindow::metaObject() const
{
    return &staticMetaObject;
}

void *MainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_MainWindow))
        return static_cast<void*>(const_cast< MainWindow*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int MainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: about(); break;
        case 1: toSend(); break;
        case 2: toLoadWeb(); break;
        case 3: toInputForm(); break;
        case 4: toInputConsole(); break;
        case 5: toInputSign(); break;
        case 6: toInputConfigure(); break;
        case 7: toDelAllWorkflow(); break;
        case 8: selInputTab((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 9: doQuit(); break;
        case 10: doGeneralOptions(); break;
        case 11: doWidgetsOptions(); break;
        case 12: setToInputForm(); break;
        case 13: setToInputConsole(); break;
        case 14: setToInputManagementSignDocument(); break;
        case 15: showSmartMenuWidget((*reinterpret_cast< DockSbMenu::ShowAction(*)>(_a[1]))); break;
        case 16: showSmartMenuWidget(); break;
        case 17: showShowResultWidget((*reinterpret_cast< DockSbMenu::ShowAction(*)>(_a[1]))); break;
        case 18: showShowResultWidget(); break;
        case 19: drawWorkflow((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< GraphicsSvgItemLive*(*)>(_a[2]))); break;
        case 20: drawWorkflow((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 21: toSbMenu(); break;
        case 22: linkClickedSbMenu((*reinterpret_cast< const QUrl(*)>(_a[1]))); break;
        case 23: linkClickedSbResult((*reinterpret_cast< const QUrl(*)>(_a[1]))); break;
        case 24: setEnabledToolBar((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 25: setEnabledToolBar(); break;
        case 26: goPrincipal(); break;
        case 27: executeJSCodeAfterLoad((*reinterpret_cast< bool(*)>(_a[1]))); break;
        }
        _id -= 28;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
