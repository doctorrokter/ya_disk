/****************************************************************************
** Meta object code from reading C++ file 'FileController.hpp'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.6)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../src/controllers/FileController.hpp"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'FileController.hpp' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_FileController[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      25,   14, // methods
       1,  139, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
      10,       // signalCount

 // signals: signature, parameters, type, tag, flags
      21,   16,   15,   15, 0x05,
      60,   46,   15,   15, 0x05,
      88,   46,   15,   15, 0x05,
     137,  116,   15,   15, 0x05,
     182,  165,   15,   15, 0x05,
     217,  165,   15,   15, 0x05,
     272,  251,   15,   15, 0x05,
     320,  310,   15,   15, 0x05,
     360,  344,   15,   15, 0x05,
     400,  394,   15,   15, 0x05,

 // slots: signature, parameters, type, tag, flags
     427,   15,   15,   15, 0x08,
     436,   15,   15,   15, 0x08,
     451,   15,   15,   15, 0x08,
     466,   15,   15,   15, 0x08,
     498,  487,   15,   15, 0x08,
     530,   15,   15,   15, 0x08,

 // methods: signature, parameters, type, tag, flags
     554,  549,   15,   15, 0x02,
     572,   15,   15,   15, 0x22,
     583,   46,   15,   15, 0x02,
     609,   46,   15,   15, 0x02,
     655,  635,   15,   15, 0x02,
     682,  165,   15,   15, 0x02,
     715,  165,   15,   15, 0x02,
     774,  748,   15,   15, 0x02,
     811,   15,  798,   15, 0x02,

 // properties: name, type, flags
     394,  798, 0x09495001,

 // properties: notify_signal_id
       9,

       0        // eod
};

static const char qt_meta_stringdata_FileController[] = {
    "FileController\0\0data\0dataLoaded(QVariantList)\0"
    "filename,path\0fileLoaded(QString,QString)\0"
    "fileOpened(QString,QString)\0"
    "direname,currentPath\0dirCreated(QString,QString)\0"
    "name,currentPath\0deletionRequested(QString,QString)\0"
    "fileOrDirDeleted(QString,QString)\0"
    "remoteUri,sent,total\0"
    "uploadProgress(QString,qint64,qint64)\0"
    "remoteUri\0uploadFinished(QString)\0"
    "targetPath,file\0fileUploaded(QString,QVariantMap)\0"
    "queue\0queueChanged(QVariantList)\0"
    "onLoad()\0onFileLoaded()\0onDirCreated()\0"
    "onFileOrDirDeleted()\0sent,total\0"
    "onUploadProgress(qint64,qint64)\0"
    "onUploadFinished()\0path\0loadPath(QString)\0"
    "loadPath()\0loadFile(QString,QString)\0"
    "openFile(QString,QString)\0dirname,currentPath\0"
    "createDir(QString,QString)\0"
    "requestDeletion(QString,QString)\0"
    "deleteFileOrDir(QString,QString)\0"
    "sourceFilePath,targetPath\0"
    "upload(QString,QString)\0QVariantList\0"
    "getQueue()\0"
};

void FileController::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        FileController *_t = static_cast<FileController *>(_o);
        switch (_id) {
        case 0: _t->dataLoaded((*reinterpret_cast< const QVariantList(*)>(_a[1]))); break;
        case 1: _t->fileLoaded((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 2: _t->fileOpened((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 3: _t->dirCreated((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 4: _t->deletionRequested((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 5: _t->fileOrDirDeleted((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 6: _t->uploadProgress((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< qint64(*)>(_a[2])),(*reinterpret_cast< qint64(*)>(_a[3]))); break;
        case 7: _t->uploadFinished((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 8: _t->fileUploaded((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QVariantMap(*)>(_a[2]))); break;
        case 9: _t->queueChanged((*reinterpret_cast< const QVariantList(*)>(_a[1]))); break;
        case 10: _t->onLoad(); break;
        case 11: _t->onFileLoaded(); break;
        case 12: _t->onDirCreated(); break;
        case 13: _t->onFileOrDirDeleted(); break;
        case 14: _t->onUploadProgress((*reinterpret_cast< qint64(*)>(_a[1])),(*reinterpret_cast< qint64(*)>(_a[2]))); break;
        case 15: _t->onUploadFinished(); break;
        case 16: _t->loadPath((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 17: _t->loadPath(); break;
        case 18: _t->loadFile((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 19: _t->openFile((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 20: _t->createDir((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 21: _t->requestDeletion((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 22: _t->deleteFileOrDir((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 23: _t->upload((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 24: { QVariantList _r = _t->getQueue();
            if (_a[0]) *reinterpret_cast< QVariantList*>(_a[0]) = _r; }  break;
        default: ;
        }
    }
}

const QMetaObjectExtraData FileController::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject FileController::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_FileController,
      qt_meta_data_FileController, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &FileController::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *FileController::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *FileController::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_FileController))
        return static_cast<void*>(const_cast< FileController*>(this));
    return QObject::qt_metacast(_clname);
}

int FileController::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 25)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 25;
    }
#ifndef QT_NO_PROPERTIES
      else if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< QVariantList*>(_v) = getQueue(); break;
        }
        _id -= 1;
    } else if (_c == QMetaObject::WriteProperty) {
        _id -= 1;
    } else if (_c == QMetaObject::ResetProperty) {
        _id -= 1;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 1;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 1;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 1;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 1;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 1;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}

// SIGNAL 0
void FileController::dataLoaded(const QVariantList & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void FileController::fileLoaded(const QString & _t1, const QString & _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void FileController::fileOpened(const QString & _t1, const QString & _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void FileController::dirCreated(const QString & _t1, const QString & _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void FileController::deletionRequested(const QString & _t1, const QString & _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void FileController::fileOrDirDeleted(const QString & _t1, const QString & _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 6
void FileController::uploadProgress(const QString & _t1, qint64 _t2, qint64 _t3)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 6, _a);
}

// SIGNAL 7
void FileController::uploadFinished(const QString & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 7, _a);
}

// SIGNAL 8
void FileController::fileUploaded(const QString _t1, const QVariantMap _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 8, _a);
}

// SIGNAL 9
void FileController::queueChanged(const QVariantList & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 9, _a);
}
QT_END_MOC_NAMESPACE
