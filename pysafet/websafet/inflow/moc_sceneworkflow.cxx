/****************************************************************************
** Meta object code from reading C++ file 'sceneworkflow.h'
**
** Created: Mon Oct 19 11:50:36 2009
**      by: The Qt Meta Object Compiler version 59 (Qt 4.4.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "sceneworkflow.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'sceneworkflow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.4.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_SceneWorkflow[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // signals: signature, parameters, type, tag, flags
      20,   15,   14,   14, 0x05,

       0        // eod
};

static const char qt_meta_stringdata_SceneWorkflow[] = {
    "SceneWorkflow\0\0item\0itemSelected(QGraphicsItem*)\0"
};

const QMetaObject SceneWorkflow::staticMetaObject = {
    { &QGraphicsScene::staticMetaObject, qt_meta_stringdata_SceneWorkflow,
      qt_meta_data_SceneWorkflow, 0 }
};

const QMetaObject *SceneWorkflow::metaObject() const
{
    return &staticMetaObject;
}

void *SceneWorkflow::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_SceneWorkflow))
        return static_cast<void*>(const_cast< SceneWorkflow*>(this));
    return QGraphicsScene::qt_metacast(_clname);
}

int SceneWorkflow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QGraphicsScene::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: itemSelected((*reinterpret_cast< QGraphicsItem*(*)>(_a[1]))); break;
        }
        _id -= 1;
    }
    return _id;
}

// SIGNAL 0
void SceneWorkflow::itemSelected(QGraphicsItem * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
