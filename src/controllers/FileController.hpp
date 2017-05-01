/*
 * FileController.hpp
 *
 *  Created on: Apr 29, 2017
 *      Author: misha
 */

#ifndef FILECONTROLLER_HPP_
#define FILECONTROLLER_HPP_

#include <QObject>
#include <QVariantList>
#include <QVariantMap>
#include <QList>
#include <QNetworkReply>
#include <bb/cascades/ProgressIndicator>
#include "../webdav/qwebdav.h"
#include "../webdav/qwebdavdirparser.h"
#include "../Common.hpp"
#include "../util/FileUtil.hpp"

using namespace bb::cascades;

class FileController: public QObject {
    Q_OBJECT
    Q_PROPERTY(QVariantList queue READ getQueue NOTIFY queueChanged)
public:
    FileController(FileUtil* fileUtil, QObject* parent = 0);
    virtual ~FileController();

    Q_INVOKABLE void loadPath(const QString& path = "/");
    Q_INVOKABLE void loadFile(const QString& filename, const QString& path);
    Q_INVOKABLE void openFile(const QString& filename, const QString& path);
    Q_INVOKABLE void createDir(const QString& dirname, const QString& currentPath);
    Q_INVOKABLE void requestDeletion(const QString& name, const QString& currentPath);
    Q_INVOKABLE void deleteFileOrDir(const QString& name, const QString& currentPath);
    Q_INVOKABLE void upload(const QString& sourceFilePath, const QString& targetPath);
    Q_INVOKABLE QVariantList getQueue();

    void initWebdav(QWebdav* webdav, QWebdavDirParser* parser);

    Q_SIGNALS:
        void dataLoaded(const QVariantList& data);
        void fileLoaded(const QString& filename, const QString& path);
        void fileOpened(const QString& filename, const QString& path);
        void dirCreated(const QString& direname, const QString& currentPath);
        void deletionRequested(const QString& name, const QString& currentPath);
        void fileOrDirDeleted(const QString& name, const QString& currentPath);
        void uploadProgress(const QString& remoteUri, qint64 sent, qint64 total);
        void uploadFinished(const QString& remoteUri);
        void queueChanged(const QVariantList& queue);

    private slots:
        void onLoad();
        void onFileLoaded();
        void onDirCreated();
        void onFileOrDirDeleted();
        void onUploadProgress(qint64 sent, qint64 total);
        void onUploadFinished();
private:
    QWebdav* m_pWebdav;
    QWebdavDirParser* m_pParser;

    FileUtil* m_pFileUtil;
    QList<QNetworkReply*> m_replies;
    QList<QNetworkReply*> m_uploadReplies;
    QVariantList m_queue;

    void startUpload(const QString& remoteUri);
};

#endif /* FILECONTROLLER_HPP_ */
