/*
 * FileDownloader.cpp
 *
 *  Created on: May 24, 2017
 *      Author: misha
 */

#include <src/controllers/loaders/FileDownloader.hpp>
#include <QDir>
#include <QFile>

FileDownloader::FileDownloader(QWebdav* webdav, QObject* parent) : QObject(parent), m_pWebdav(webdav) {}

FileDownloader::~FileDownloader() {}

QVariantList FileDownloader::getQueue() const {
    QVariantList list;
    foreach(Download d, m_readyQueue) {
        list.append(d.toMap());
    }
    return list;
}

void FileDownloader::download(const QString& filename, const QString& path) {
    Download download;
    download.setFilename(filename).setPath(path);
    m_readyQueue.append(download);

    emit addedToQueue(download.toMap());

    if (m_readyQueue.size() <= DOWNLOADS_QUEUE_SIZE) {
        m_downloadsQueue.append(download);
        startDownload(filename, path);
    }
}

void FileDownloader::startDownload(const QString& filename, const QString& path) {
    QNetworkReply* reply = m_pWebdav->get(path);
    reply->setProperty("filename", filename);
    reply->setProperty("path", path);

    #ifdef DEBUG_WEBDAV
        qDebug() << "FileDownloader ===>>> Download file: " << path << endl;
    #endif

    bool res = QObject::connect(reply, SIGNAL(finished()), this, SLOT(onFileDownloaded()));
    Q_ASSERT(res);
    res = QObject::connect(reply, SIGNAL(downloadProgress(qint64, qint64)), this, SLOT(onDownloadProgress(qint64, qint64)));
    Q_ASSERT(res);
    Q_UNUSED(res);
}

void FileDownloader::onFileDownloaded() {
    QNetworkReply* reply = qobject_cast<QNetworkReply*>(QObject::sender());
    QObject::disconnect(reply, SIGNAL(downloadProgress(qint64, qint64)), this, SLOT(onDownloadProgress(qint64, qint64)));

    QString filename = reply->property("filename").toString();
    QString path = reply->property("path").toString();
    QByteArray data = reply->readAll();

    QString downloadsPath = QDir::currentPath() + "/shared/downloads/yadisk";
    QDir dir(downloadsPath);
    if (!dir.exists()) {
        bool res = dir.mkpath(downloadsPath);
        qDebug() << "Dir " << downloadsPath << " created: " << res << endl;
    }

    QFile file(downloadsPath + "/" + filename);
    if (file.open(QIODevice::WriteOnly)) {
        file.write(data);
        file.close();
    } else {
        qDebug() << "Can't open a file: " << file.errorString() << endl;
    }

    removeFromQueue(path, m_readyQueue);
    removeFromQueue(path, m_downloadsQueue);

    if (m_readyQueue.size() != 0) {
        Download download;
        for (int i = 0; i < m_readyQueue.size(); i++) {
            Download d = m_readyQueue.at(i);
            if (!isDownloading(d.getPath())) {
                download = d;
            }
        }
        if (!download.getPath().isEmpty()) {
            m_downloadsQueue.append(download);
            startDownload(download.getFilename(), download.getPath());
        }
    }

    reply->deleteLater();
    emit finish(filename, path);
}

void FileDownloader::onDownloadProgress(qint64 recevied, qint64 total) {
    QNetworkReply* reply = qobject_cast<QNetworkReply*>(QObject::sender());
    QString filename = reply->property("filename").toString();
    QString path = reply->property("path").toString();
    emit downloadProgress(path, recevied, total);
}

bool FileDownloader::isDownloading(const QString& path) {
    foreach(Download d, m_downloadsQueue) {
        if (d.getPath().compare(path) == 0) {
            return true;
        }
    }
    return false;
}

void FileDownloader::removeFromQueue(const QString& path, QList<Download>& queue) {
    for (int i = 0; i < queue.size(); i++) {
        if (queue.at(i).getPath().compare(path) == 0) {
            queue.removeAt(i);
        }
    }
}
