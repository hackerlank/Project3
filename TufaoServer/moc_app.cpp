/****************************************************************************
** Meta object code from reading C++ file 'app.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.2.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "app.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'app.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.2.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_App_t {
    QByteArrayData data[12];
    char stringdata[202];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    offsetof(qt_meta_stringdata_App_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData) \
    )
static const qt_meta_stringdata_App_t qt_meta_stringdata_App = {
    {
QT_MOC_LITERAL(0, 0, 3),
QT_MOC_LITERAL(1, 4, 21),
QT_MOC_LITERAL(2, 26, 0),
QT_MOC_LITERAL(3, 27, 12),
QT_MOC_LITERAL(4, 40, 24),
QT_MOC_LITERAL(5, 65, 16),
QT_MOC_LITERAL(6, 82, 25),
QT_MOC_LITERAL(7, 108, 7),
QT_MOC_LITERAL(8, 116, 26),
QT_MOC_LITERAL(9, 143, 8),
QT_MOC_LITERAL(10, 152, 22),
QT_MOC_LITERAL(11, 175, 25)
    },
    "App\0sigPassengerUpdatepos\0\0QJsonObject&\0"
    "sigPassengerOrderrequest\0slotRequestReady\0"
    "Tufao::HttpServerRequest&\0request\0"
    "Tufao::HttpServerResponse&\0response\0"
    "slotPassengerUpdatepos\0slotPassengerOrderrequest\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_App[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   39,    2, 0x06,
       4,    2,   42,    2, 0x06,

 // slots: name, argc, parameters, tag, flags
       5,    2,   47,    2, 0x0a,
      10,    1,   52,    2, 0x0a,
      11,    2,   55,    2, 0x0a,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    2,
    QMetaType::Void, 0x80000000 | 3, 0x80000000 | 3,    2,    2,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 6, 0x80000000 | 8,    7,    9,
    QMetaType::Void, 0x80000000 | 3,    2,
    QMetaType::Void, 0x80000000 | 3, 0x80000000 | 3,    2,    2,

       0        // eod
};

void App::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        App *_t = static_cast<App *>(_o);
        switch (_id) {
        case 0: _t->sigPassengerUpdatepos((*reinterpret_cast< QJsonObject(*)>(_a[1]))); break;
        case 1: _t->sigPassengerOrderrequest((*reinterpret_cast< QJsonObject(*)>(_a[1])),(*reinterpret_cast< QJsonObject(*)>(_a[2]))); break;
        case 2: _t->slotRequestReady((*reinterpret_cast< Tufao::HttpServerRequest(*)>(_a[1])),(*reinterpret_cast< Tufao::HttpServerResponse(*)>(_a[2]))); break;
        case 3: _t->slotPassengerUpdatepos((*reinterpret_cast< QJsonObject(*)>(_a[1]))); break;
        case 4: _t->slotPassengerOrderrequest((*reinterpret_cast< QJsonObject(*)>(_a[1])),(*reinterpret_cast< QJsonObject(*)>(_a[2]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (App::*_t)(QJsonObject & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&App::sigPassengerUpdatepos)) {
                *result = 0;
            }
        }
        {
            typedef void (App::*_t)(QJsonObject & , QJsonObject & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&App::sigPassengerOrderrequest)) {
                *result = 1;
            }
        }
    }
}

const QMetaObject App::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_App.data,
      qt_meta_data_App,  qt_static_metacall, 0, 0}
};


const QMetaObject *App::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *App::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_App.stringdata))
        return static_cast<void*>(const_cast< App*>(this));
    return QObject::qt_metacast(_clname);
}

int App::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 5)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 5;
    }
    return _id;
}

// SIGNAL 0
void App::sigPassengerUpdatepos(QJsonObject & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void App::sigPassengerOrderrequest(QJsonObject & _t1, QJsonObject & _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_END_MOC_NAMESPACE
