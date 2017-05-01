/****************************************************************************
** Meta object code from reading C++ file 'qwebdav.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.6)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../src/webdav/qwebdav.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'qwebdav.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_QWebdav[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: signature, parameters, type, tag, flags
      16,    9,    8,    8, 0x05,
      59,   53,    8,    8, 0x05,

 // slots: signature, parameters, type, tag, flags
      81,    8,    8,    8, 0x09,
      98,    8,    8,    8, 0x09,
     128,    8,    8,    8, 0x09,
     161,    8,    8,    8, 0x09,
     214,  201,    8,    8, 0x09,

       0        // eod
};

static const char qt_meta_stringdata_QWebdav[] = {
    "QWebdav\0\0errors\0checkSslCertifcate(QList<QSslError>)\0"
    "error\0errorChanged(QString)\0"
    "replyReadyRead()\0replyFinished(QNetworkReply*)\0"
    "replyDeleteLater(QNetworkReply*)\0"
    "replyError(QNetworkReply::NetworkError)\0"
    "reply,errors\0sslErrors(QNetworkReply*,QList<QSslError>)\0"
};

void QWebdav::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        QWebdav *_t = static_cast<QWebdav *>(_o);
        switch (_id) {
        case 0: _t->checkSslCertifcate((*reinterpret_cast< const QList<QSslError>(*)>(_a[1]))); break;
        case 1: _t->errorChanged((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 2: _t->replyReadyRead(); break;
        case 3: _t->replyFinished((*reinterpret_cast< QNetworkReply*(*)>(_a[1]))); break;
        case 4: _t->replyDeleteLater((*reinterpret_cast< QNetworkReply*(*)>(_a[1]))); break;
        case 5: _t->replyError((*reinterpret_cast< QNetworkReply::NetworkError(*)>(_a[1]))); break;
        case 6: _t->sslErrors((*reinterpret_cast< QNetworkReply*(*)>(_a[1])),(*reinterpret_cast< const QList<QSslError>(*)>(_a[2]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData QWebdav::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject QWebdav::staticMetaObject = {
    { &QNetworkAccessManager::staticMetaObject, qt_meta_stringdata_QWebdav,
      qt_meta_data_QWebdav, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &QWebdav::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *QWebdav::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *QWebdav::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_QWebdav))
        return static_cast<void*>(const_cast< QWebdav*>(this));
    return QNetworkAccessManager::qt_metacast(_clname);
}

int QWebdav::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QNetworkAccessManager::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 7)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    }
    return _id;
}

// SIGNAL 0
void QWebdav::checkSslCertifcate(const QList<QSslError> & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void QWebdav::errorChanged(QString _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_END_MOC_NAMESPACE
