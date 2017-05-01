/*
 * AppConfig.cpp
 *
 *  Created on: Jan 17, 2017
 *      Author: misha
 */

#include "AppConfig.hpp"
#include <QUrl>
#include <QDir>
#include <QDebug>

QSettings AppConfig::CONF;

AppConfig::AppConfig(QObject* parent) : QObject(parent) {
    m_publicAssetsPath = QUrl("file://" + QDir::currentPath() + "/app/public").toString();
}

AppConfig::~AppConfig() {}

QVariant AppConfig::getStatic(const QString name) {
    return CONF.value(name, "");
}

void AppConfig::setStatic(const QString name, const QVariant value) {
    CONF.setValue(name, value);
}

QVariant AppConfig::get(const QString name) const {
    return getStatic(name);
}

void AppConfig::set(const QString name, const QVariant value) {
    setStatic(name, value);
    emit settingsChanged();
}

QString AppConfig::getPublicAssets() const { return m_publicAssetsPath; }
