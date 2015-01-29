/****************************************************************************
** Meta object code from reading C++ file 'pages.h'
**
** Created: Mon Oct 19 11:50:37 2009
**      by: The Qt Meta Object Compiler version 59 (Qt 4.4.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "pages.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'pages.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.4.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_ConfigurationPage[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       0,    0, // enums/sets

       0        // eod
};

static const char qt_meta_stringdata_ConfigurationPage[] = {
    "ConfigurationPage\0"
};

const QMetaObject ConfigurationPage::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_ConfigurationPage,
      qt_meta_data_ConfigurationPage, 0 }
};

const QMetaObject *ConfigurationPage::metaObject() const
{
    return &staticMetaObject;
}

void *ConfigurationPage::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_ConfigurationPage))
        return static_cast<void*>(const_cast< ConfigurationPage*>(this));
    return QWidget::qt_metacast(_clname);
}

int ConfigurationPage::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}
static const uint qt_meta_data_QueryPage[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       0,    0, // enums/sets

       0        // eod
};

static const char qt_meta_stringdata_QueryPage[] = {
    "QueryPage\0"
};

const QMetaObject QueryPage::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_QueryPage,
      qt_meta_data_QueryPage, 0 }
};

const QMetaObject *QueryPage::metaObject() const
{
    return &staticMetaObject;
}

void *QueryPage::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_QueryPage))
        return static_cast<void*>(const_cast< QueryPage*>(this));
    return QWidget::qt_metacast(_clname);
}

int QueryPage::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}
static const uint qt_meta_data_UpdatePage[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // slots: signature, parameters, type, tag, flags
      12,   11,   11,   11, 0x08,
      27,   11,   11,   11, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_UpdatePage[] = {
    "UpdatePage\0\0startService()\0stopService()\0"
};

const QMetaObject UpdatePage::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_UpdatePage,
      qt_meta_data_UpdatePage, 0 }
};

const QMetaObject *UpdatePage::metaObject() const
{
    return &staticMetaObject;
}

void *UpdatePage::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_UpdatePage))
        return static_cast<void*>(const_cast< UpdatePage*>(this));
    return QWidget::qt_metacast(_clname);
}

int UpdatePage::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: startService(); break;
        case 1: stopService(); break;
        }
        _id -= 2;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
