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
#include "../webdav/qwebdav.h"
#include "../webdav/qwebdavdirparser.h"
#include "../Common.hpp"
#include "../util/FileUtil.hpp"

class FileController: public QObject {
    Q_OBJECT
    Q_PROPERTY(QVariantList queue READ getQueue NOTIFY queueChanged)
    Q_PROPERTY(QVariantList selectedFiles READ getSelectedFiles NOTIFY selectedFilesChanged)
    Q_PROPERTY(QString currentPath READ getCurrentPath WRITE setCurrentPath NOTIFY currentPathChanged)
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
    Q_INVOKABLE void rename(const QString& currentName, const QString& currentPath, const QString& newName, const bool& isDir, const QString& ext = "");
    Q_INVOKABLE void move(const QString& name, const QString& fromPath, const QString& toPath, const bool& isDir, const QString& ext = "");
    Q_INVOKABLE QVariantList getQueue();
    Q_INVOKABLE QVariantList getSelectedFiles() const;
    Q_INVOKABLE void selectFile(const QVariantMap& file);
    Q_INVOKABLE void clearSelectedFiles();
    Q_INVOKABLE const QString& getCurrentPath() const;
    Q_INVOKABLE void setCurrentPath(const QString& currentPath);

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
        void fileUploaded(const QString targetPath, const QVariantMap file);
        void fileRenamed(const QString& prevName, const QString& prevPath, const QString& newName, const QString& namePath);
        void fileMoved(const QString& name, const QString& prevPath, const QString& newPath, const QString& currentPath, const bool& isDir, const QString& ext);
        void queueChanged(const QVariantList& queue);
        void selectedFilesChanged(const QVariantList& selectedFiles);
        void currentPathChanged(const QString& currentPath);

    private slots:
        void onLoad();
        void onFileLoaded();
        void onDirCreated();
        void onFileOrDirDeleted();
        void onUploadProgress(qint64 sent, qint64 total);
        void onUploadFinished();
        void onFileRenamed();
        void onFileMoved();
private:
    QWebdav* m_pWebdav;
    QWebdavDirParser* m_pParser;

    FileUtil* m_pFileUtil;
    QList<QNetworkReply*> m_replies;
    QList<QNetworkReply*> m_uploadReplies;
    QVariantList m_queue;
    QVariantList m_selectedFiles;
    QString m_currentPath;

    void startUpload(const QString& remoteUri);
};

#endif /* FILECONTROLLER_HPP_ */
