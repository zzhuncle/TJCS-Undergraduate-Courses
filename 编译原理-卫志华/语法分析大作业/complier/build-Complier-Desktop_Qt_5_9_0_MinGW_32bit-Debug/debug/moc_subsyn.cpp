/****************************************************************************
** Meta object code from reading C++ file 'subsyn.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.9.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../../../../../complier_1/ComplierProject/subsyn.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'subsyn.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_subsyn_t {
    QByteArrayData data[12];
    char stringdata0[234];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_subsyn_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_subsyn_t qt_meta_stringdata_subsyn = {
    {
QT_MOC_LITERAL(0, 0, 6), // "subsyn"
QT_MOC_LITERAL(1, 7, 18), // "RETURNTOMAINWINDOW"
QT_MOC_LITERAL(2, 26, 0), // ""
QT_MOC_LITERAL(3, 27, 8), // "CLOSEALL"
QT_MOC_LITERAL(4, 36, 24), // "on_pushButton_5_released"
QT_MOC_LITERAL(5, 61, 22), // "on_pushButton_released"
QT_MOC_LITERAL(6, 84, 24), // "on_pushButton_3_released"
QT_MOC_LITERAL(7, 109, 24), // "on_pushButton_2_released"
QT_MOC_LITERAL(8, 134, 24), // "on_pushButton_8_released"
QT_MOC_LITERAL(9, 159, 24), // "on_pushButton_4_released"
QT_MOC_LITERAL(10, 184, 24), // "on_pushButton_6_released"
QT_MOC_LITERAL(11, 209, 24) // "on_pushButton_7_released"

    },
    "subsyn\0RETURNTOMAINWINDOW\0\0CLOSEALL\0"
    "on_pushButton_5_released\0"
    "on_pushButton_released\0on_pushButton_3_released\0"
    "on_pushButton_2_released\0"
    "on_pushButton_8_released\0"
    "on_pushButton_4_released\0"
    "on_pushButton_6_released\0"
    "on_pushButton_7_released"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_subsyn[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      10,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   64,    2, 0x06 /* Public */,
       3,    0,   65,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       4,    0,   66,    2, 0x08 /* Private */,
       5,    0,   67,    2, 0x08 /* Private */,
       6,    0,   68,    2, 0x08 /* Private */,
       7,    0,   69,    2, 0x08 /* Private */,
       8,    0,   70,    2, 0x08 /* Private */,
       9,    0,   71,    2, 0x08 /* Private */,
      10,    0,   72,    2, 0x08 /* Private */,
      11,    0,   73,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void subsyn::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        subsyn *_t = static_cast<subsyn *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->RETURNTOMAINWINDOW(); break;
        case 1: _t->CLOSEALL(); break;
        case 2: _t->on_pushButton_5_released(); break;
        case 3: _t->on_pushButton_released(); break;
        case 4: _t->on_pushButton_3_released(); break;
        case 5: _t->on_pushButton_2_released(); break;
        case 6: _t->on_pushButton_8_released(); break;
        case 7: _t->on_pushButton_4_released(); break;
        case 8: _t->on_pushButton_6_released(); break;
        case 9: _t->on_pushButton_7_released(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (subsyn::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&subsyn::RETURNTOMAINWINDOW)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (subsyn::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&subsyn::CLOSEALL)) {
                *result = 1;
                return;
            }
        }
    }
    Q_UNUSED(_a);
}

const QMetaObject subsyn::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_subsyn.data,
      qt_meta_data_subsyn,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *subsyn::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *subsyn::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_subsyn.stringdata0))
        return static_cast<void*>(const_cast< subsyn*>(this));
    return QWidget::qt_metacast(_clname);
}

int subsyn::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 10)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 10;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 10)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 10;
    }
    return _id;
}

// SIGNAL 0
void subsyn::RETURNTOMAINWINDOW()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void subsyn::CLOSEALL()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
