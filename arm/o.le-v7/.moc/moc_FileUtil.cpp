/****************************************************************************
** Meta object code from reading C++ file 'FileUtil.hpp'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.6)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../src/util/FileUtil.hpp"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'FileUtil.hpp' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_FileUtil[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // methods: signature, parameters, type, tag, flags
      19,   15,   10,    9, 0x02,
      36,   15,   10,    9, 0x02,
      53,   15,   10,    9, 0x02,
      70,   15,   10,    9, 0x02,
      85,   15,   10,    9, 0x02,
     108,   15,   10,    9, 0x02,
     132,   15,   10,    9, 0x02,
     164,  155,  147,    9, 0x02,

       0        // eod
};

static const char qt_meta_stringdata_FileUtil[] = {
    "FileUtil\0\0bool\0ext\0isImage(QString)\0"
    "isVideo(QString)\0isAudio(QString)\0"
    "isDoc(QString)\0isSpreadSheet(QString)\0"
    "isPresentation(QString)\0isPdf(QString)\0"
    "QString\0filepath\0filename(QString)\0"
};

void FileUtil::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        FileUtil *_t = static_cast<FileUtil *>(_o);
        switch (_id) {
        case 0: { bool _r = _t->isImage((*reinterpret_cast< const QString(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 1: { bool _r = _t->isVideo((*reinterpret_cast< const QString(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 2: { bool _r = _t->isAudio((*reinterpret_cast< const QString(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 3: { bool _r = _t->isDoc((*reinterpret_cast< const QString(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 4: { bool _r = _t->isSpreadSheet((*reinterpret_cast< const QString(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 5: { bool _r = _t->isPresentation((*reinterpret_cast< const QString(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 6: { bool _r = _t->isPdf((*reinterpret_cast< const QString(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 7: { QString _r = _t->filename((*reinterpret_cast< const QString(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = _r; }  break;
        default: ;
        }
    }
}

const QMetaObjectExtraData FileUtil::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject FileUtil::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_FileUtil,
      qt_meta_data_FileUtil, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &FileUtil::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *FileUtil::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *FileUtil::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_FileUtil))
        return static_cast<void*>(const_cast< FileUtil*>(this));
    return QObject::qt_metacast(_clname);
}

int FileUtil::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 8)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 8;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
