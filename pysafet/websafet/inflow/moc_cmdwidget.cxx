/****************************************************************************
** Meta object code from reading C++ file 'cmdwidget.h'
**
** Created: Mon Oct 19 11:50:36 2009
**      by: The Qt Meta Object Compiler version 59 (Qt 4.4.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "cmdwidget.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'cmdwidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.4.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_CmdWidget[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // slots: signature, parameters, type, tag, flags
      19,   11,   10,   10, 0x0a,
      41,   36,   10,   10, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_CmdWidget[] = {
    "CmdWidget\0\0newText\0setText(QString)\0"
    "text\0changeLblValidator(QString)\0"
};

const QMetaObject CmdWidget::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_CmdWidget,
      qt_meta_data_CmdWidget, 0 }
};

const QMetaObject *CmdWidget::metaObject() const
{
    return &staticMetaObject;
}

void *CmdWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CmdWidget))
        return static_cast<void*>(const_cast< CmdWidget*>(this));
    return QWidget::qt_metacast(_clname);
}

int CmdWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: setText((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 1: changeLblValidator((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        }
        _id -= 2;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
