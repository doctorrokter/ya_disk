/*
 * PreviewLoader.cpp
 *
 *  Created on: May 30, 2017
 *      Author: misha
 */

#include "PreviewLoader.hpp"

PreviewLoader::PreviewLoader(QWebdav* webdav, QObject* parent) : QObject(parent), m_pWebdav(webdav) {}

PreviewLoader::~PreviewLoader() {}

QVariantList PreviewLoader::getQueue() const {
    QVariantList list;
    foreach(Download d, m_queue) {
        list.append(d.toMap());
    }
    return list;
}

void PreviewLoader::load(const QString& filename, const QString& path) {
    QString previewLocalPath = QDir::currentPath() + PREVIEWS_DIR + path;
    QFile preview(previewLocalPath);
    if (preview.exists() && preview.size() > 1000) {
        emit loaded(path, previewLocalPath);
    } else {
        Download download;
        download.setFilename(filename).setPath(path);
        m_queue.append(download);
        if (m_queue.size() <= PREVIEWS_QUEUE_SIZE) {
            m_downloadsQueue.append(download);
            startLoad(filename, path);
        }
    }
}

void PreviewLoader::onLoad() {
    QNetworkReply* reply = qobject_cast<QNetworkReply*>(QObject::sender());
    QObject::disconnect(reply, SIGNAL(finished()), this, SLOT(onLoad()));

    QByteArray bytes = reply->readAll();

    QString filename = reply->property("filename").toString();
    QString path = reply->property("path").toString();

    removeFromQueue(path, m_queue);

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
        emit loaded(path, filePath);
    }

    reply->deleteLater();

    if (m_queue.size() != 0) {
        Download download;
        foreach(Download d, m_queue) {
            if (!isDownloading(d.getPath())) {
                download = d;
            }
        }
        if (!download.getPath().isEmpty()) {
            m_downloadsQueue.append(download);
            startLoad(download.getFilename(), download.getPath());
        }
    }
}

void PreviewLoader::startLoad(const QString& filename, const QString& path) {
    QNetworkReply* reply = m_pWebdav->preview(path, "XS");
    reply->setProperty("filename", filename);
    reply->setProperty("path", path);

    bool res = QObject::connect(reply, SIGNAL(finished()), this, SLOT(onLoad()));
    Q_ASSERT(res);
    Q_UNUSED(res);
}

void PreviewLoader::removeFromQueue(const QString& path, QList<Download>& queue) {
    for (int i = 0; i < queue.size(); i++) {
        if (queue.at(i).getPath().compare(path) == 0) {
            queue.removeAt(i);
        }
    }
}

bool PreviewLoader::isDownloading(const QString& path) {
    foreach(Download d, m_downloadsQueue) {
        if (d.getPath().compare(path) == 0) {
            return true;
        }
    }
    return false;
}
