/*
 * Download.cpp
 *
 *  Created on: May 25, 2017
 *      Author: misha
 */

#include "Download.hpp"
#include <QDebug>

Download::Download(QObject* parent) : QObject(parent), m_savePath(""), m_filename(""), m_path(""), m_received(0), m_total(0) {}

Download::Download(const Download& download) : QObject(download.parent()) {
    swap(download);
}

Download::~Download() {}

Download& Download::operator=(const Download& download) {
    swap(download);
    return *this;
}

bool Download::operator==(const Download& download) {
    return this->getPath().compare(download.getPath()) == 0;
}

const QString& Download::getSavePath() const { return m_savePath; }
Download& Download::setSavePath(const QString& savePath) {
    m_savePath = savePath;
    return *this;
}

const QString& Download::getFilename() const { return m_filename; }
Download& Download::setFilename(const QString& filename) {
    m_filename = filename;
    return *this;
}

const QString& Download::getPath() const { return m_path; }
Download& Download::setPath(const QString& path) {
    m_path = path;
    return *this;
}

const qint64& Download::getReceived() const { return m_received; }
Download& Download::setReceived(const qint64& received) {
    m_received = received;
    return *this;
}

const qint64& Download::getTotal() const { return m_total; }
Download& Download::setTotal(const qint64& total) {
    m_total = total;
    return *this;
}

QVariantMap Download::toMap() const {
    QVariantMap map;
    map["save_path"] = m_savePath;
    map["filename"] = m_filename;
    map["path"] = m_path;
    map["received"] = m_received;
    map["total"] = m_total;
    return map;
}

void Download::swap(const Download& download) {
    QString savePath = download.getSavePath();
    this->setSavePath(savePath);

    QString filename = download.getFilename();
    this->setFilename(filename);

    QString path = download.getPath();
    this->setPath(path);

    qint64 received = download.getReceived();
    this->setReceived(received);

    qint64 total = download.getTotal();
    this->setTotal(total);
}
