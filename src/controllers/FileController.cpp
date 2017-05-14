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
#include <bb/data/XmlDataAccess>
#include "../webdav/qwebdavitem.h"

using namespace bb::system;
using namespace bb::data;

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

void FileController::loadPath(const QString& path, const int& amount, const int& offset) {
    bool res = m_pParser->listDirectory(m_pWebdav, path, amount, offset);
    Q_ASSERT(res);
    Q_UNUSED(res);
}

void FileController::loadFile(const QString& filename, const QString& path) {
    QNetworkReply* reply = m_pWebdav->get(path);
    reply->setProperty("filename", filename);
    reply->setProperty("path", path);
    m_replies.append(reply);

    #ifdef DEBUG_WEBDAV
        qDebug() << "FileController ===>>>  Load file: " << path << endl;
    #endif

    bool res = QObject::connect(reply, SIGNAL(finished()), this, SLOT(onFileLoaded()));
    Q_ASSERT(res);
    Q_UNUSED(res);
}

void FileController::openFile(const QString& filename, const QString& path) {
    QString fullUrl = QDir::currentPath() + TEMP_DIR + path;
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
        dataList.append(item.toMap());
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

    QString tempDir = QDir::currentPath() + TEMP_DIR + pathCopy.replace(QString("/").append(filename), "");
    QDir dir(tempDir);
    if (!dir.exists()) {
        dir.mkpath(tempDir);
    }

    QString filePath = QDir::currentPath() + TEMP_DIR + path;
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

    if (m_uploadReplies.size() < UPLOADS_QUEUE_SIZE) {
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
    file["size"] = reply->property("size").toUInt();
    file["lastModified"] = QDateTime::currentDateTime();
    QStringList filenameParts = filename.split(".");
    if (filenameParts.size() > 1) {
        file["ext"] = filenameParts[filenameParts.size() - 1];
    } else {
        file["ext"] = "";
    }
    emit fileUploaded(reply->property("targetPath").toString(), file);

    m_uploadReplies.removeAll(reply);
    reply->deleteLater();

    if (m_queue.size() > 0) {
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

    QString sourceFilePath = map.value("sourceFilePath").toString().replace("file://", "");
    QFile file(sourceFilePath);
    if (file.exists()) {
        QNetworkReply* reply = m_pWebdav->put(remoteUri, file);
        reply->setProperty("filename", map.value("filename").toString());
        reply->setProperty("remoteUri", remoteUri);
        reply->setProperty("targetPath", map.value("targetPath").toString());
        reply->setProperty("size", file.size());

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

void FileController::rename(const QString& currentName, const QString& currentPath, const QString& newName, const bool& isDir, const QString& ext) {
    QString destPath;
    QString currPath = currentPath;
    QString fullNewName = newName;

    if (isDir) {
        destPath = currPath.replace(currentName + "/", "").append(newName).append("/");
    } else {
        fullNewName.append(".").append(ext.toLower());
        destPath = currPath.replace(currentName, "", Qt::CaseInsensitive).append(fullNewName);
    }

    QNetworkReply* reply = m_pWebdav->move(currentPath, destPath);
    reply->setProperty("prevName", currentName);
    reply->setProperty("prevPath", currentPath);
    reply->setProperty("newName", fullNewName);
    reply->setProperty("newPath", destPath);

    bool res = QObject::connect(reply, SIGNAL(finished()), this, SLOT(onFileRenamed()));
    Q_ASSERT(res);
    Q_UNUSED(res);
    m_replies.append(reply);
}

void FileController::onFileRenamed() {
    QNetworkReply* reply = qobject_cast<QNetworkReply*>(QObject::sender());

    #ifdef DEBUG_WEBDAV
        qDebug() << "FileController ===>>> file rename result: " << reply->readAll().data() << endl;
    #endif

    emit fileRenamed(
            reply->property("prevName").toString(),
            reply->property("prevPath").toString(),
            reply->property("newName").toString(),
            reply->property("newPath").toString()
    );

    m_replies.removeAll(reply);
    reply->deleteLater();
}

void FileController::move(const QString& name, const QString& fromPath, const QString& toPath, const bool& isDir, const QString& ext) {
    QString newPath = toPath + name;

    QNetworkReply* reply = m_pWebdav->move(fromPath, newPath, true);
    reply->setProperty("prevPath", fromPath);
    reply->setProperty("newPath", newPath);
    reply->setProperty("currentPath", toPath);
    reply->setProperty("name", name);
    reply->setProperty("dir", isDir);
    reply->setProperty("ext", ext);

    bool res = QObject::connect(reply, SIGNAL(finished()), this, SLOT(onFileMoved()));
    Q_ASSERT(res);
    Q_UNUSED(res);
    m_replies.append(reply);
}

void FileController::onFileMoved() {
    QNetworkReply* reply = qobject_cast<QNetworkReply*>(QObject::sender());

    #ifdef DEBUG_WEBDAV
        qDebug() << "FileController ===>>> file move result: " << reply->readAll().data() << endl;
    #endif

    emit fileMoved(
            reply->property("name").toString(),
            reply->property("prevPath").toString(),
            reply->property("newPath").toString(),
            reply->property("currentPath").toString(),
            reply->property("dir").toBool(),
            reply->property("ext").toString()
    );

    m_replies.removeAll(reply);
    reply->deleteLater();
}

QVariantList FileController::getSelectedFiles() const {
    return m_selectedFiles;
}

void FileController::selectFile(const QVariantMap& file) {
    m_selectedFiles.append(file);
    emit selectedFilesChanged(m_selectedFiles);
}

void FileController::clearSelectedFiles() {
    m_selectedFiles.clear();
    emit selectedFilesChanged(m_selectedFiles);
}

const QString& FileController::getCurrentPath() const {
    return m_currentPath;
}

void FileController::setCurrentPath(const QString& currentPath) {
    m_currentPath = currentPath;
    emit currentPathChanged(m_currentPath);
}

void FileController::showProps(const QVariantMap& fileMap) {
    emit propsPageRequested(fileMap);
}

void FileController::loadPreview(const QString& filename, const QString& path) {
    QString previewLocalPath = QDir::currentPath() + PREVIEWS_DIR + path;
    QFile preview(previewLocalPath);
    if (preview.exists() && preview.size() > 1000) {
        emit previewLoaded(path, previewLocalPath);
    } else {
        QVariantMap previewMap;
        previewMap["filename"] = filename;
        previewMap["path"] = path;
        m_previewsQueue.append(previewMap);
        if (m_previewsQueue.size() <= PREVIEWS_QUEUE_SIZE) {
            startLoadPreview(filename, path);
        }
    }
}

void FileController::onPreviewLoaded() {
    QNetworkReply* reply = qobject_cast<QNetworkReply*>(QObject::sender());
    QByteArray bytes = reply->readAll();

    QString filename = reply->property("filename").toString();
    QString path = reply->property("path").toString();

    for (int i = 0; i< m_previewsQueue.size(); i++) {
        QVariantMap previewMap = m_previewsQueue.at(i).toMap();
        if (previewMap.value("filename").toString().compare(filename) == 0 && previewMap.value("path").toString().compare(path) == 0) {
            m_previewsQueue.removeAt(i);
        }
    }

    if (bytes.size() != 0) {
        QString pathCopy = QString(path);

        QString previewsDir = QDir::currentPath() + PREVIEWS_DIR + pathCopy.replace(QString("/").append(filename), "");
        QDir dir(previewsDir);
        if (!dir.exists()) {
            dir.mkpath(previewsDir);
        }

        QString filePath = QDir::currentPath() + PREVIEWS_DIR + path;
        QFile file(filePath);

        if (file.open(QIODevice::WriteOnly)) {
            file.write(bytes);
            file.close();

            #ifdef DEBUG_WEBDAV
                qDebug() << "FileController ===>>>  Preview saved to " << filePath << ", size: " << file.size() << endl;
            #endif
        }
        emit previewLoaded(path, filePath);
    }

    m_previewReplies.removeAll(reply);
    reply->deleteLater();

    if (m_previewsQueue.size() > 0) {
        QVariantMap previewMap = m_previewsQueue.at(0).toMap();
        startLoadPreview(previewMap.value("filename").toString(), previewMap.value("path").toString());
    }
}

void FileController::startLoadPreview(const QString& filename, const QString& path) {
    QNetworkReply* reply = m_pWebdav->preview(path, "XS");
    reply->setProperty("filename", filename);
    reply->setProperty("path", path);
    m_previewReplies.append(reply);

    bool res = QObject::connect(reply, SIGNAL(finished()), this, SLOT(onPreviewLoaded()));
    Q_ASSERT(res);
    Q_UNUSED(res);
}

const QVariantList& FileController::getSharedFiles() const { return m_sharedFiles; }
void FileController::setSharedFiles(const QVariantList& sharedFiles) {
    m_sharedFiles = sharedFiles;
    emit sharedFilesChanged(m_sharedFiles);
}

void FileController::clearSharedFiles() {
    m_sharedFiles.clear();
    emit sharedFilesChanged(m_sharedFiles);
}

void FileController::makePublic(const QString& path) {
    QWebdav::PropValues props;
    QMap<QString, QVariant> map;
    map.insert("public_url", "true");
    props.insert("urn:yandex:disk:meta", map);

    QNetworkReply* reply = m_pWebdav->propertyupdate(path, props);
    reply->setProperty("path", path);

    bool res = QObject::connect(reply, SIGNAL(finished()), this, SLOT(onPublicMade()));
    Q_ASSERT(res);
    Q_UNUSED(res);
}

void FileController::onPublicMade() {
    QNetworkReply* reply = qobject_cast<QNetworkReply*>(QObject::sender());
    QByteArray bytes = reply->readAll();

    qDebug() << "FileController ===>>> makePublic " << bytes.data() << endl;

    XmlDataAccess xda;
    QVariantMap map = xda.loadFromBuffer(bytes, "/d:multistatus/d:response/d:propstat/d:prop").toMap();
    QString publicUrl = map.value("public_url").toMap().value(".data").toString();
    QString path = reply->property("path").toString();

    reply->deleteLater();
    emit publicMade(path, publicUrl);
}
