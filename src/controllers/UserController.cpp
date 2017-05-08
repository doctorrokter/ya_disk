/*
 * UserController.cpp
 *
 *  Created on: May 5, 2017
 *      Author: misha
 */

#include <src/controllers/UserController.hpp>
#include <bb/data/XmlDataAccess>

using namespace bb::data;

UserController::UserController(QObject* parent) : QObject(parent), m_pWebdav(0), m_pParser(0), m_pUser(0) {}

UserController::~UserController() {
    m_pWebdav->deleteLater();
    m_pParser->deleteLater();
    m_pUser->deleteLater();
}

void UserController::initWebdav(QWebdav* webdav, QWebdavDirParser* parser) {
    m_pWebdav = webdav;
    m_pParser = parser;
}

void UserController::userinfo() {
    QNetworkReply* reply = m_pWebdav->userinfo();

    bool res = QObject::connect(reply, SIGNAL(finished()), this, SLOT(onUserinfoLoaded()));
    Q_ASSERT(res);
    Q_UNUSED(res);
}

void UserController::onUserinfoLoaded() {
    QNetworkReply* reply = qobject_cast<QNetworkReply*>(QObject::sender());
    QString data = reply->readAll().data();

    m_pUser = new User(this);
    QStringList lnParts = data.split("\n");
    foreach(QString part, lnParts) {
        QString p = part.trimmed();
        if (!p.isEmpty()) {
            qDebug() << p << endl;
            QStringList keyVal = p.split(":");
            QString key = keyVal[0];
            QString val = keyVal[1];
            if (key.compare("uid") == 0) {
                m_pUser->setUid(val.toInt());
            } else if (key.compare("login") == 0) {
                m_pUser->setLogin(val);
            } else if (key.compare("fio") == 0) {
                m_pUser->setFio(val);
            } else if (key.compare("firstname") == 0) {
                m_pUser->setFirstname(val);
            } else if (key.compare("lastname") == 0) {
                m_pUser->setLastname(val);
            } else if (key.compare("upload_concurrency") == 0) {
                m_pUser->setUploadConcurrency(val.toInt());
            } else if (key.compare("datasync_db_prefix") == 0) {
                m_pUser->setDatasyncDbPrefix(val);
            } else if (key.compare("is_b2b") == 0) {
                m_pUser->setB2b(val.compare("true") == 0);
            }
        }
    }
    reply->deleteLater();
    emit userChanged(m_pUser->toMap());
}

User* UserController::getUser() const { return m_pUser; }

void UserController::diskinfo() {
    QWebdav::PropNames props;
    QStringList values;
    values << "quota-available-bytes" << "quota-used-bytes";
    props.insert("DAV:", values);

    QNetworkReply* reply = m_pWebdav->propfind("/", props);

    bool res = QObject::connect(reply, SIGNAL(finished()), this, SLOT(onDiskinfoLoaded()));
    Q_ASSERT(res);
    Q_UNUSED(res);
}

void UserController::onDiskinfoLoaded() {
    QNetworkReply* reply = qobject_cast<QNetworkReply*>(QObject::sender());

    QString data = reply->readAll().data();

    XmlDataAccess xda;
    QVariantMap map = xda.loadFromBuffer(data, "/d:multistatus/d:response/d:propstat/d:prop").toMap();

    m_pUser->setAvailableBytes(map.value("d:quota-available-bytes").toLongLong());
    m_pUser->setUsedBytes(map.value("d:quota-used-bytes").toLongLong());

    reply->deleteLater();
    emit userChanged(m_pUser->toMap());
}
