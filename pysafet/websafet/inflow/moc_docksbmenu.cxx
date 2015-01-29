/****************************************************************************
** Meta object code from reading C++ file 'docksbmenu.h'
**
** Created: Mon Oct 19 11:50:37 2009
**      by: The Qt Meta Object Compiler version 59 (Qt 4.4.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "docksbmenu.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'docksbmenu.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.4.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_BlueTitleBar[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // slots: signature, parameters, type, tag, flags
      14,   13,   13,   13, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_BlueTitleBar[] = {
    "BlueTitleBar\0\0updateMask()\0"
};

const QMetaObject BlueTitleBar::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_BlueTitleBar,
      qt_meta_data_BlueTitleBar, 0 }
};

const QMetaObject *BlueTitleBar::metaObject() const
{
    return &staticMetaObject;
}

void *BlueTitleBar::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_BlueTitleBar))
        return static_cast<void*>(const_cast< BlueTitleBar*>(this));
    return QWidget::qt_metacast(_clname);
}

int BlueTitleBar::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: updateMask(); break;
        }
        _id -= 1;
    }
    return _id;
}
static const uint qt_meta_data_DockSbMenu[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       0,    0, // enums/sets

       0        // eod
};

static const char qt_meta_stringdata_DockSbMenu[] = {
    "DockSbMenu\0"
};

const QMetaObject DockSbMenu::staticMetaObject = {
    { &QDockWidget::staticMetaObject, qt_meta_stringdata_DockSbMenu,
      qt_meta_data_DockSbMenu, 0 }
};

const QMetaObject *DockSbMenu::metaObject() const
{
    return &staticMetaObject;
}

void *DockSbMenu::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_DockSbMenu))
        return static_cast<void*>(const_cast< DockSbMenu*>(this));
    return QDockWidget::qt_metacast(_clname);
}

int DockSbMenu::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDockWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}
QT_END_MOC_NAMESPACE
