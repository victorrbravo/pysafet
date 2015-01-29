/****************************************************************************
** Meta object code from reading C++ file 'infloweditqlistwidget.h'
**
** Created: Mon Oct 19 11:50:37 2009
**      by: The Qt Meta Object Compiler version 59 (Qt 4.4.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "infloweditqlistwidget.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'infloweditqlistwidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.4.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_InflowEditQListWidget[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // slots: signature, parameters, type, tag, flags
      23,   22,   22,   22, 0x08,
      33,   22,   22,   22, 0x08,
      43,   22,   22,   22, 0x08,
      54,   22,   22,   22, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_InflowEditQListWidget[] = {
    "InflowEditQListWidget\0\0delItem()\0"
    "addItem()\0editItem()\0areItemsEnabled()\0"
};

const QMetaObject InflowEditQListWidget::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_InflowEditQListWidget,
      qt_meta_data_InflowEditQListWidget, 0 }
};

const QMetaObject *InflowEditQListWidget::metaObject() const
{
    return &staticMetaObject;
}

void *InflowEditQListWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_InflowEditQListWidget))
        return static_cast<void*>(const_cast< InflowEditQListWidget*>(this));
    return QWidget::qt_metacast(_clname);
}

int InflowEditQListWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: delItem(); break;
        case 1: addItem(); break;
        case 2: editItem(); break;
        case 3: areItemsEnabled(); break;
        }
        _id -= 4;
    }
    return _id;
}
static const uint qt_meta_data_Inflow__ListDialog[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // slots: signature, parameters, type, tag, flags
      23,   20,   19,   19, 0x0a,
      57,   51,   19,   19, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_Inflow__ListDialog[] = {
    "Inflow::ListDialog\0\0id\0"
    "predefineButtonClicked(int)\0state\0"
    "setEditInfoType(bool)\0"
};

const QMetaObject Inflow::ListDialog::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_Inflow__ListDialog,
      qt_meta_data_Inflow__ListDialog, 0 }
};

const QMetaObject *Inflow::ListDialog::metaObject() const
{
    return &staticMetaObject;
}

void *Inflow::ListDialog::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Inflow__ListDialog))
        return static_cast<void*>(const_cast< ListDialog*>(this));
    return QDialog::qt_metacast(_clname);
}

int Inflow::ListDialog::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: predefineButtonClicked((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: setEditInfoType((*reinterpret_cast< bool(*)>(_a[1]))); break;
        }
        _id -= 2;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
