/****************************************************************************
** Meta object code from reading C++ file 'dommodel.h'
**
** Created: Mon Oct 19 11:50:36 2009
**      by: The Qt Meta Object Compiler version 59 (Qt 4.4.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "dommodel.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'dommodel.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.4.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_DomModel[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       0,    0, // enums/sets

       0        // eod
};

static const char qt_meta_stringdata_DomModel[] = {
    "DomModel\0"
};

const QMetaObject DomModel::staticMetaObject = {
    { &QStringListModel::staticMetaObject, qt_meta_stringdata_DomModel,
      qt_meta_data_DomModel, 0 }
};

const QMetaObject *DomModel::metaObject() const
{
    return &staticMetaObject;
}

void *DomModel::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_DomModel))
        return static_cast<void*>(const_cast< DomModel*>(this));
    return QStringListModel::qt_metacast(_clname);
}

int DomModel::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QStringListModel::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}
QT_END_MOC_NAMESPACE
