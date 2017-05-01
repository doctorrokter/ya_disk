/*
 * FileController.cpp
 *
 *  Created on: Apr 29, 2017
 *      Author: misha
 */

#include <src/controllers/FileController.hpp>
#include <QVariantList>
#include <QStringList>
#include <QVariantMap>
#include <QDir>
#include <QUrl>
#include <QFile>
#include <bb/system/InvokeRequest>
#include <bb/system/InvokeManager>
#include "../webdav/qwebdavitem.h"

using namespace bb::system;

FileController::FileController(FileUtil* fileUtil, QObject* parent) : QObject(parent),
    m_pWebdav(0), m_pParser(0), m_pFileUtil(fileUtil) {}

FileController::~FileController() {
    m_pWebdav->deleteLater();
    m_pParser->deleteLater();
    m_pFileUtil->deleteLater();
}

void FileController::initWebdav(QWebdav* webdav, QWebdavDirParser* parser) {
    m_pWebdav = webdav;
    m_pParser = parser;

    bool res = QObject::connect(m_pParser, SIGNAL(finished()), this, SLOT(onLoad()));
    Q_ASSERT(res);
    Q_UNUSED(res);

    loadPath();
}

void FileController::loadPath(const QString& path) {
    bool res = m_pParser->listDirectory(m_pWebdav, path);
    Q_ASSERT(res);
    Q_UNUSED(res);
}

void FileController::loadFile(const QString& filename, const QString& path) {
    QNetworkReply* reply = m_pWebdav->get(path);
    reply->setProperty("filename", filename);
    reply->setProperty("path", path);
    m_replies.append(reply);

    qDebug() << "FileController ===>>>  Load file: " << path << endl;

    bool res = QObject::connect(reply, SIGNAL(finished()), this, SLOT(onFileLoaded()));
    Q_ASSERT(res);
    Q_UNUSED(res);
}

void FileController::openFile(const QString& filename, const QString& path) {
    QString fullUrl = QDir::currentPath() + "/data" + path;
    QFile file(fullUrl);

    if (!file.exists()) {
        loadFile(filename, path);
    } else {
        InvokeManager invokeManager;
        InvokeRequest request;

        QUrl url(QString("file://").append(fullUrl));

        request.setAction("bb.action.VIEW");
        request.setUri(url);

        QStringList parts = filename.split(".");
        QString ext = parts.at(parts.size() - 1);

        if (m_pFileUtil->isPdf(ext)) {
            request.setTarget("com.rim.bb.app.adobeReader.viewer");
        } else if (m_pFileUtil->isImage(ext)) {
            request.setTarget("sys.pictures.card.previewer");
        } else if (m_pFileUtil->isDoc(ext)) {
            request.setTarget("sys.wordtogo.previewer");
        } else if (m_pFileUtil->isSpreadSheet(ext)) {
            request.setTarget("sys.sheettogo.previewer");
        } else if (m_pFileUtil->isPresentation(ext)) {
            request.setTarget("sys.slideshowtogo.previewer");
        } else if (m_pFileUtil->isAudio(ext) || m_pFileUtil->isVideo(ext)) {
            request.setTarget("sys.mediaplayer.previewer");
        }

        if (!request.target().isEmpty()) {
            invokeManager.invoke(request);
        }
        emit fileOpened(filename, path);
    }
}

void FileController::onLoad() {
    QList<QWebdavItem> list = m_pParser->getList();
    QVariantList dataList;
    QWebdavItem item;
    foreach(item, list) {
        QVariantMap m;
        m["path"] = item.path();
        m["name"] = item.name();
        m["ext"] = item.ext();
        m["dir"] = item.isDir();
        m["createdAt"] = item.createdAt();
        m["createdAtStr"] = item.createdAtStr();
        m["lastModified"] = item.lastModified();
        m["lastModifiedStr"] = item.lastModifiedStr();
        m["mimeType"] = item.mimeType();

        #ifdef DEBUG_WEBDAV
            qDebug() << m << endl;
        #endif

        dataList.append(m);
    }
    emit dataLoaded(dataList);
}

void FileController::onFileLoaded() {
    QNetworkReply* reply = qobject_cast<QNetworkReply*>(QObject::sender());

    QString filename = reply->property("filename").toString();
    QString path = reply->property("path").toString();
    QString pathCopy = QString(path);

    #ifdef DEBUG_WEBDAV
        qDebug() << "FileController ===>>>  File loaded: " << filename << " " << path << endl;
    #endif

    QString tempDir = QDir::currentPath() + "/data" + pathCopy.replace(QString("/").append(filename), "");
    QDir dir(tempDir);
    if (!dir.exists()) {
        dir.mkpath(tempDir);
    }

    QString filePath = QDir::currentPath() + "/data" + path;
    QFile file(filePath);
    QByteArray bytes = reply->readAll();

    if (file.open(QIODevice::WriteOnly)) {
        file.write(bytes);
        file.close();

        #ifdef DEBUG_WEBDAV
            qDebug() << "FileController ===>>>  File saved to " << filePath << endl;
        #endif
    }

    m_replies.removeAll(reply);
    reply->deleteLater();

    openFile(filename, path);
    emit fileLoaded(filename, path);
}

void FileController::createDir(const QString& dirname, const QString& currentPath) {
    QNetworkReply* reply = m_pWebdav->mkdir(currentPath + "/" + dirname);
    reply->setProperty("dirname", dirname);
    reply->setProperty("currentPath", currentPath);
    m_replies.append(reply);

    bool res = QObject::connect(reply, SIGNAL(finished()), this, SLOT(onDirCreated()));
    Q_ASSERT(res);
    Q_UNUSED(res);
}

void FileController::onDirCreated() {
    QNetworkReply* reply = qobject_cast<QNetworkReply*>(QObject::sender());
    QString dirname = reply->property("dirname").toString();
    QString currentPath = reply->property("currentPath").toString();

    QString response = reply->readAll().data();

    m_replies.removeAll(reply);
    reply->deleteLater();
    emit dirCreated(dirname, currentPath);
}

void FileController::requestDeletion(const QString& name, const QString& currentPath) {
    emit deletionRequested(name, currentPath);
}

void FileController::deleteFileOrDir(const QString& name, const QString& currentPath) {
    QNetworkReply* reply = m_pWebdav->remove(currentPath);
    reply->setProperty("name", name);
    reply->setProperty("currentPath", currentPath);
    m_replies.append(reply);

    bool res = QObject::connect(reply, SIGNAL(finished()), this, SLOT(onFileOrDirDeleted()));
    Q_ASSERT(res);
    Q_UNUSED(res);
}

void FileController::onFileOrDirDeleted() {
    QNetworkReply* reply = qobject_cast<QNetworkReply*>(QObject::sender());
    QString name = reply->property("name").toString();
    QString currentPath = reply->property("currentPath").toString();

    QString response = reply->readAll().data();

    m_replies.removeAll(reply);
    reply->deleteLater();
    emit fileOrDirDeleted(name, currentPath);
}

void FileController::upload(const QString& sourceFilePath, const QString& targetPath) {
    QString filename = m_pFileUtil->filename(sourceFilePath);
    QString remoteUri = QString(targetPath).append(filename);

    QVariantMap map;
    map["filename"] = filename;
    map["remoteUri"] = remoteUri;
    map["targetPath"] = targetPath;
    map["sourceFilePath"] = sourceFilePath;
    m_queue.append(map);

    emit queueChanged(m_queue);

    if (m_uploadReplies.size() < 2) {
        startUpload(remoteUri);
    }
}

void FileController::onUploadProgress(qint64 sent, qint64 total) {
    QNetworkReply* reply = qobject_cast<QNetworkReply*>(QObject::sender());
    QString remoteUri = reply->property("remoteUri").toString();
    emit uploadProgress(remoteUri, sent, total);
}

void FileController::onUploadFinished() {
    QNetworkReply* reply = qobject_cast<QNetworkReply*>(QObject::sender());
    QString remoteUri = reply->property("remoteUri").toString();

    #ifdef DEBUG_WEBDAV
        qDebug() << "FileController ===>>> upload result: " << reply->readAll() << endl;
    #endif

    for (int i = 0; i < m_queue.size(); i++) {
        QVariantMap map = m_queue.at(0).toMap();
        if (map.value("remoteUri").toString().compare(remoteUri) == 0) {
            m_queue.removeAt(i);
        }
    }

    QVariantMap file;
    QString filename = m_pFileUtil->filename(remoteUri);
    file["path"] = remoteUri;
    file["name"] = filename;
    file["dir"] = false;
    file["lastModified"] = QDateTime::currentDateTime();
    QStringList filenameParts = filename.split(".");
    if (filenameParts.size() > 1) {
        file["ext"] = filenameParts[1];
    } else {
        file["ext"] = "";
    }
    emit fileUploaded(reply->property("targetPath").toString(), file);

    m_uploadReplies.removeAll(reply);
    reply->deleteLater();

    if (m_queue.size() > 1) {
        startUpload(m_queue.at(0).toMap().value("remoteUri").toString());
        startUpload(m_queue.at(1).toMap().value("remoteUri").toString());
    } else if (m_queue.size() == 1) {
        startUpload(m_queue.at(0).toMap().value("remoteUri").toString());
    }

    emit uploadFinished(remoteUri);
}

QVariantList FileController::getQueue() {
    return m_queue;
}

void FileController::startUpload(const QString& remoteUri) {
    QVariantMap map;
    for (int i = 0; i < m_queue.size(); i++) {
        map = m_queue.at(0).toMap();
    }

    QString sourceFilePath = map.value("sourceFilePath").toString();
    QFile file(sourceFilePath);
    if (file.exists()) {
        QNetworkReply* reply = m_pWebdav->put(remoteUri, file);
        reply->setProperty("filename", map.value("filename").toString());
        reply->setProperty("remoteUri", remoteUri);
        reply->setProperty("targetPath", map.value("targetPath").toString());

        bool res = QObject::connect(reply, SIGNAL(uploadProgress(qint64,qint64)), this, SLOT(onUploadProgress(qint64,qint64)));
        Q_ASSERT(true);
        res = QObject::connect(reply, SIGNAL(finished()), this, SLOT(onUploadFinished()));
        Q_ASSERT(true);
        Q_UNUSED(res);

        m_uploadReplies.append(reply);
    } else {
        #ifdef DEBUG_WEBDAV
            qDebug () << "FileController ===>>> File does not exists: " << sourceFilePath << endl;
        #endif
    }
}
