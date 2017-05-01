/****************************************************************************
** Meta object code from reading C++ file 'AppConfig.hpp'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.6)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../src/config/AppConfig.hpp"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'AppConfig.hpp' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_AppConfig[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      11,   10,   10,   10, 0x05,

 // methods: signature, parameters, type, tag, flags
      43,   38,   29,   10, 0x02,
      67,   56,   10,   10, 0x02,
      97,   10,   89,   10, 0x02,

       0        // eod
};

static const char qt_meta_stringdata_AppConfig[] = {
    "AppConfig\0\0settingsChanged()\0QVariant\0"
    "name\0get(QString)\0name,value\0"
    "set(QString,QVariant)\0QString\0"
    "getPublicAssets()\0"
};

void AppConfig::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        AppConfig *_t = static_cast<AppConfig *>(_o);
        switch (_id) {
        case 0: _t->settingsChanged(); break;
        case 1: { QVariant _r = _t->get((*reinterpret_cast< const QString(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< QVariant*>(_a[0]) = _r; }  break;
        case 2: _t->set((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QVariant(*)>(_a[2]))); break;
        case 3: { QString _r = _t->getPublicAssets();
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = _r; }  break;
        default: ;
        }
    }
}

const QMetaObjectExtraData AppConfig::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject AppConfig::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_AppConfig,
      qt_meta_data_AppConfig, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &AppConfig::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *AppConfig::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *AppConfig::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_AppConfig))
        return static_cast<void*>(const_cast< AppConfig*>(this));
    return QObject::qt_metacast(_clname);
}

int AppConfig::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    }
    return _id;
}

// SIGNAL 0
void AppConfig::settingsChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}
QT_END_MOC_NAMESPACE
