/*
 * PreviewLoader.hpp
 *
 *  Created on: May 30, 2017
 *      Author: misha
 */

#ifndef PREVIEWLOADER_HPP_
#define PREVIEWLOADER_HPP_

#include <QObject>
#include "../../webdav/qwebdav.h"
#include "beans/Download.hpp"
#include "../../Common.hpp"
#include <QVariantList>
#include <QList>

class PreviewLoader: public QObject {
    Q_OBJECT
    Q_PROPERTY(QVariantList queue READ getQueue);
public:
    PreviewLoader(QWebdav* webdav, QObject* parent = 0);
    virtual ~PreviewLoader();

    Q_INVOKABLE QVariantList getQueue() const;
    Q_INVOKABLE void load(const QString& filename, const QString& path);

Q_SIGNALS:
    void loaded(const QString& path, const QString& localPreviewPath);

private slots:
    void onLoad();

private:
    QWebdav* m_pWebdav;
    QList<Download> m_queue;
    QList<Download> m_downloadsQueue;

    void startLoad(const QString& filename, const QString& path);
    bool isDownloading(const QString& path);
    void removeFromQueue(const QString& path, QList<Download>& queue);
};

#endif /* PREVIEWLOADER_HPP_ */
