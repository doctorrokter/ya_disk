/*
 * FileDownloader.hpp
 *
 *  Created on: May 24, 2017
 *      Author: misha
 */

#ifndef FILEDOWNLOADER_HPP_
#define FILEDOWNLOADER_HPP_

#include <QObject>
#include <QVariantList>
#include <QVariantMap>
#include <QList>
#include <QNetworkReply>
#include "../../webdav/qwebdav.h"
#include "../../Common.hpp"
#include "beans/Download.hpp"

class FileDownloader: public QObject {
    Q_OBJECT
    Q_PROPERTY(QVariantList queue READ getQueue);
public:
    FileDownloader(QWebdav* webdav, QObject* parent = 0);
    virtual ~FileDownloader();

    Q_INVOKABLE QVariantList getQueue() const;
    Q_INVOKABLE void download(const QString& filename, const QString& path);

Q_SIGNALS:
    void addedToQueue(const QVariantMap& command);
    void finish(const QString& filename, const QString& path);
    void downloadProgress(const QString& path, qint64 received, qint64 total);

private slots:
    void onFileDownloaded();
    void onDownloadProgress(qint64 received, qint64 total);

private:
    QWebdav* m_pWebdav;
    QList<Download> m_readyQueue;
    QList<Download> m_downloadsQueue;

    void startDownload(const QString& filename, const QString& path);
    bool isDownloading(const QString& path);
    void removeFromQueue(const QString& path, QList<Download>& queue);
};

#endif /* FILEDOWNLOADER_HPP_ */
