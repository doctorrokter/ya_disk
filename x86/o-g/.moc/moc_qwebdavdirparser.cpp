/****************************************************************************
** Meta object code from reading C++ file 'qwebdavdirparser.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.6)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../src/webdav/qwebdavdirparser.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'qwebdavdirparser.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_QWebdavDirParser[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: signature, parameters, type, tag, flags
      18,   17,   17,   17, 0x05,
      29,   17,   17,   17, 0x05,

 // slots: signature, parameters, type, tag, flags
      51,   17,   17,   17, 0x0a,
      59,   17,   17,   17, 0x09,
      81,   75,   17,   17, 0x09,

       0        // eod
};

static const char qt_meta_stringdata_QWebdavDirParser[] = {
    "QWebdavDirParser\0\0finished()\0"
    "errorChanged(QString)\0abort()\0"
    "replyFinished()\0reply\0"
    "replyDeleteLater(QNetworkReply*)\0"
};

void QWebdavDirParser::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        QWebdavDirParser *_t = static_cast<QWebdavDirParser *>(_o);
        switch (_id) {
        case 0: _t->finished(); break;
        case 1: _t->errorChanged((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 2: _t->abort(); break;
        case 3: _t->replyFinished(); break;
        case 4: _t->replyDeleteLater((*reinterpret_cast< QNetworkReply*(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData QWebdavDirParser::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject QWebdavDirParser::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_QWebdavDirParser,
      qt_meta_data_QWebdavDirParser, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &QWebdavDirParser::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *QWebdavDirParser::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *QWebdavDirParser::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_QWebdavDirParser))
        return static_cast<void*>(const_cast< QWebdavDirParser*>(this));
    return QObject::qt_metacast(_clname);
}

int QWebdavDirParser::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    }
    return _id;
}

// SIGNAL 0
void QWebdavDirParser::finished()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}

// SIGNAL 1
void QWebdavDirParser::errorChanged(QString _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_END_MOC_NAMESPACE
