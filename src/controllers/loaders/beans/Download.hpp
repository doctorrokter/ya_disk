/*
 * Download.hpp
 *
 *  Created on: May 25, 2017
 *      Author: misha
 */

#ifndef DOWNLOAD_HPP_
#define DOWNLOAD_HPP_

#include <QObject>
#include <QVariantMap>

class Download: public QObject {
    Q_OBJECT
public:
    Download(QObject* parent = 0);
    Download(const Download& download);
    virtual ~Download();

    Download& operator=(const Download& download);
    bool operator==(const Download& download);

    const QString& getSavePath() const;
    Download& setSavePath(const QString& savePath);

    const QString& getFilename() const;
    Download& setFilename(const QString& filename);

    const QString& getPath() const;
    Download& setPath(const QString& path);

    const qint64& getReceived() const;
    Download& setReceived(const qint64& received);

    const qint64& getTotal() const;
    Download& setTotal(const qint64& total);

    QVariantMap toMap() const;

private:
    QString m_savePath;
    QString m_filename;
    QString m_path;
    qint64 m_received;
    qint64 m_total;

    void swap(const Download& download);
};

#endif /* DOWNLOAD_HPP_ */
