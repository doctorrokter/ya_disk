/*
 * AppConfig.hpp
 *
 *  Created on: Jan 17, 2017
 *      Author: misha
 */

#ifndef APPCONFIG_HPP_
#define APPCONFIG_HPP_

#include <QtCore/QObject>
#include <QVariant>
#include <QtCore/QSettings>

class AppConfig: public QObject {
    Q_OBJECT

public:
    AppConfig(QObject* parent = 0);
    virtual ~AppConfig();

    static QSettings CONF;

    static QVariant getStatic(const QString name);
    static void setStatic(const QString name, const QVariant value);

    Q_INVOKABLE QVariant get(const QString name) const;
    Q_INVOKABLE void set(const QString name, const QVariant value);
    Q_INVOKABLE QString getPublicAssets() const;

Q_SIGNALS:
    void settingsChanged();

private:
    QString m_publicAssetsPath;
};

#endif /* APPCONFIG_HPP_ */
