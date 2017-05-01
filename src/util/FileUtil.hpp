/*
 * FileUtil.hpp
 *
 *  Created on: Apr 29, 2017
 *      Author: misha
 */

#ifndef FILEUTIL_HPP_
#define FILEUTIL_HPP_

#include <QObject>
#include <QStringList>

class FileUtil: public QObject {
    Q_OBJECT
public:
    FileUtil(QObject* parent = 0);
    virtual ~FileUtil();

    Q_INVOKABLE bool isImage(const QString& ext);
    Q_INVOKABLE bool isVideo(const QString& ext);
    Q_INVOKABLE bool isAudio(const QString& ext);
    Q_INVOKABLE bool isDoc(const QString& ext);
    Q_INVOKABLE bool isSpreadSheet(const QString& ext);
    Q_INVOKABLE bool isPresentation(const QString& ext);
    Q_INVOKABLE bool isPdf(const QString& ext);
    Q_INVOKABLE QString filename(const QString& filepath);

private:
    QStringList m_imagesList;
    QStringList m_videoList;
    QStringList m_audioList;
    QStringList m_docList;
    QStringList m_xlsList;
    QStringList m_pptList;
};

#endif /* FILEUTIL_HPP_ */
