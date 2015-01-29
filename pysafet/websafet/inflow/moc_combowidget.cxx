/****************************************************************************
** Meta object code from reading C++ file 'combowidget.h'
**
** Created: Mon Oct 19 11:50:36 2009
**      by: The Qt Meta Object Compiler version 59 (Qt 4.4.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "combowidget.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'combowidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.4.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_ComboWidget[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // slots: signature, parameters, type, tag, flags
      21,   12,   13,   12, 0x0a,
      36,   28,   12,   12, 0x0a,
      53,   12,   12,   12, 0x0a,
      67,   12,   12,   12, 0x0a,
      94,   12,   12,   12, 0x0a,
     117,   12,   12,   12, 0x0a,
     142,   12,   12,   12, 0x0a,
     160,   12,   12,   12, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_ComboWidget[] = {
    "ComboWidget\0\0QString\0text()\0newText\0"
    "setText(QString)\0updateCombo()\0"
    "updateComboVariableSafet()\0"
    "updateComboListTable()\0updateComboListLiteral()\0"
    "updateComboFlow()\0viewdoc()\0"
};

const QMetaObject ComboWidget::staticMetaObject = {
    { &CmdWidget::staticMetaObject, qt_meta_stringdata_ComboWidget,
      qt_meta_data_ComboWidget, 0 }
};

const QMetaObject *ComboWidget::metaObject() const
{
    return &staticMetaObject;
}

void *ComboWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_ComboWidget))
        return static_cast<void*>(const_cast< ComboWidget*>(this));
    return CmdWidget::qt_metacast(_clname);
}

int ComboWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = CmdWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: { QString _r = text();
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = _r; }  break;
        case 1: setText((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 2: updateCombo(); break;
        case 3: updateComboVariableSafet(); break;
        case 4: updateComboListTable(); break;
        case 5: updateComboListLiteral(); break;
        case 6: updateComboFlow(); break;
        case 7: viewdoc(); break;
        }
        _id -= 8;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
