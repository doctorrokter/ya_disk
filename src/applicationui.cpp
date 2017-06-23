/*
 * Copyright (c) 2011-2015 BlackBerry Limited.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "applicationui.hpp"

#include <bb/cascades/Application>
#include <bb/cascades/QmlDocument>
#include <bb/cascades/AbstractPane>
#include <bb/cascades/LocaleHandler>
#include <bb/cascades/ThemeSupport>
#include <bb/system/CardDoneMessage>
#include <QVariantList>
#include <QStringList>
#include <QVariantMap>
#include <QDir>
#include <QFile>
#include <QList>
#include <bb/data/JsonDataAccess>
#include <bb/system/Clipboard>
#include <QNetworkReply>
#include <QNetworkRequest>

using namespace bb::cascades;
using namespace bb::data;
using namespace bb::system;

ApplicationUI::ApplicationUI() : QObject() {

    QCoreApplication::setOrganizationName("mikhail.chachkouski");
    QCoreApplication::setApplicationName("YaDisk");

    if (m_settings.value("expires_time_stamp", "").toString().compare("") != 0) {
        QDateTime expiryDate = QDateTime::fromTime_t(m_settings.value("expires_time_stamp").toInt());
        if (expiryDate <= QDateTime::currentDateTime()) {
            flushToken();
        }
    }

    if (m_settings.value("device_id", "").toString().isEmpty()) {
        flushToken();
    }

    m_pAppConfig = new AppConfig(this);
    m_pTranslator = new QTranslator(this);
    m_pLocaleHandler = new LocaleHandler(this);
    m_pFileUtil = new FileUtil(this);
    m_pDateUtil = new DateUtil(this);
    m_pFileController = new FileController(m_pFileUtil, this);
    m_pUserController = new UserController(this);

    QString tempDirPath = QDir::currentPath() + TEMP_DIR;
    m_pFileUtil->removeDir(tempDirPath);

    QString urlsDirPath = QDir::currentPath() + PUBLIC_URLS_DIR;
    m_pFileUtil->removeDir(urlsDirPath);
    QDir urlsDir;
    urlsDir.mkpath(urlsDirPath);

    bool res = QObject::connect(m_pLocaleHandler, SIGNAL(systemLanguageChanged()), this, SLOT(onSystemLanguageChanged()));
    Q_ASSERT(res);
    Q_UNUSED(res);

    onSystemLanguageChanged();

    m_pInvokeManager = new InvokeManager(this);
    connect(m_pInvokeManager, SIGNAL(invoked(const bb::system::InvokeRequest&)), SLOT(onInvoked(const bb::system::InvokeRequest&)));

    switch (m_pInvokeManager->startupMode()) {
            case ApplicationStartupMode::LaunchApplication:
                m_startupMode = "Launch";
                initFullUI();
                break;
            case ApplicationStartupMode::InvokeApplication:
                m_startupMode = "Invoke";
                break;
            case ApplicationStartupMode::InvokeCard:
                m_startupMode = "Card";
                break;
            }
}

ApplicationUI::~ApplicationUI() {
    m_pAppConfig->deleteLater();
    m_pFileController->deleteLater();
    m_pFileUtil->deleteLater();
    m_pUserController->deleteLater();
    m_pInvokeManager->deleteLater();
    m_pLocaleHandler->deleteLater();
    m_pParser->deleteLater();
    m_pTranslator->deleteLater();
    m_pWebdav->deleteLater();
    m_pDateUtil->deleteLater();
}

void ApplicationUI::onSystemLanguageChanged() {
    QCoreApplication::instance()->removeTranslator(m_pTranslator);
    QLocale locale;
    QString locale_string = locale.name();

    QString file_name = QString("YaDisk_%1").arg(locale_string);
    if (m_pTranslator->load(file_name, "app/native/qm")) {
        QCoreApplication::instance()->installTranslator(m_pTranslator);
    }
}

void ApplicationUI::setToken(const QString& token, const int& expiresIn) {
    m_settings.setValue("access_token", token);
    m_settings.setValue("expires_in", expiresIn);
    m_settings.setValue("expires_time_stamp", QDateTime::currentDateTime().addSecs(expiresIn).toTime_t());
    initWebdav();
    qDebug() << "Token received: " << token << ", expires in: " << expiresIn << endl;
}

bool ApplicationUI::hasToken() {
    return m_settings.contains("access_token");
}

void ApplicationUI::initWebdav() {
    m_pWebdav = new QWebdav(this);
    m_pWebdav->setConnectionSettings(QWebdav::HTTPS, QString(API_URL), "/", m_settings.value("access_token").toString(), QString(API_PORT).toInt());
    m_pParser = new QWebdavDirParser(this);
    m_pFileController->initWebdav(m_pWebdav, m_pParser);
    m_pUserController->initWebdav(m_pWebdav, m_pParser);
}

void ApplicationUI::initFullUI(const QString& data, const QString& mimeType) {
    m_pAppConfig = new AppConfig(this);
        QString theme = m_pAppConfig->get("theme").toString();
        if (theme.compare("") != 0) {
            if (theme.compare("DARK") == 0) {
                Application::instance()->themeSupport()->setVisualStyle(VisualStyle::Dark);
            } else {
                Application::instance()->themeSupport()->setVisualStyle(VisualStyle::Bright);
            }
        }

    QmlDocument *qml = QmlDocument::create("asset:///main.qml").parent(this);
    QDeclarativeEngine* engine = QmlDocument::defaultDeclarativeEngine();
    QDeclarativeContext* rootContext = engine->rootContext();
    rootContext->setContextProperty("_app", this);
    rootContext->setContextProperty("_appConfig", m_pAppConfig);
    rootContext->setContextProperty("_file", m_pFileUtil);
    rootContext->setContextProperty("_date", m_pDateUtil);
    rootContext->setContextProperty("_fileController", m_pFileController);
    rootContext->setContextProperty("_userController", m_pUserController);
    rootContext->setContextProperty("_data", data);
    rootContext->setContextProperty("_mimeType", mimeType);

    if (m_settings.value("device_id", "").toString().isEmpty()) {
        QUuid uuid;
        QString uuidStr = uuid.createUuid().toString();
        rootContext->setContextProperty("_deviceId", uuidStr);
        m_settings.setValue("device_id", uuidStr);
    } else {
        rootContext->setContextProperty("_deviceId", m_settings.value("device_id", "").toString());
    }

    QLocale systemLocale;
    QString yaDomain = "com";
    if (systemLocale.language() == QLocale::Russian) {
        yaDomain = "ru";
    }
    rootContext->setContextProperty("_yaDomain", yaDomain);

    AbstractPane *root = qml->createRootObject<AbstractPane>();

    if (hasToken()) {
        initWebdav();
    }
    Application::instance()->setScene(root);
}

void ApplicationUI::onCardDone(const QString& msg) {
    CardDoneMessage message;
    message.setData(msg);
    message.setDataType("text/plain");
    message.setReason(tr("Success!"));

    m_pInvokeManager->sendCardDone(message);
    emit cardDone();
}

void ApplicationUI::onInvoked(const bb::system::InvokeRequest& request) {
    QString action = request.action();
    QString target = request.target();
    QString mimeType = request.mimeType();

#ifdef DEBUG_WEBDAV
    qDebug() << "action: " << action << endl;
    qDebug() << "target: " << target << endl;
    qDebug() << "mimeType: " << mimeType << endl;
#endif

    if (target == INVOKE_CARD_EDIT_URI) {
        QByteArray data = request.data();
        QString uri = request.uri().toString();
        QVariantList list;
        if (uri.contains("list")) {
            JsonDataAccess jda;
            QVariant var = jda.loadFromBuffer(data);
            QVariantList dataList = var.toList();
            foreach(QVariant v, dataList) {
                QVariantMap m = v.toMap();
                list << m.value("uri").toString();
            }
        } else {
            list << uri;
        }
        m_pFileController->setSharedFiles(list);
        initFullUI();
    }
}

bool ApplicationUI::copyToClipboard(const QString& str) {
    Clipboard clipboard;
    clipboard.clear();
    QByteArray data = str.toLatin1();
    return clipboard.insert("text/plain", data);
}

void ApplicationUI::logout() {
    QNetworkRequest req;
    QUrl url("https://" + QString("oauth.yandex.ru") + "/revoke_token");
    req.setUrl(url);

    QUrl params;
    params.addQueryItem("access_token", m_settings.value("access_token").toString());
    params.addQueryItem("device_id", m_settings.value("device_id").toString().replace("{", "").replace("}", ""));

    QByteArray secret;
    secret.append(QString(CLIENT_ID).append(":").append(CLIENT_SECRET));

    QByteArray header;
    header.append(QString("Basic ").append(secret.toBase64().data()));
    req.setRawHeader("Authorization", header);
    req.setRawHeader("Content-Type", "application/x-www-form-urlencoded");

    qDebug() << "===>>> LOGOUT <<<===" << endl;
    qDebug() << req.url().toString() << endl;
    qDebug() << req.rawHeader("Authorization") << endl;
    qDebug() << params.toString() << endl;

    m_pNetwork = new QNetworkAccessManager(this);
    QNetworkReply* reply = m_pNetwork->post(req, params.encodedQuery());
    bool res = QObject::connect(reply, SIGNAL(finished()), this, SLOT(onLogout()));
    Q_ASSERT(res);
    Q_UNUSED(res);
}

void ApplicationUI::onLogout() {
    QNetworkReply* reply = qobject_cast<QNetworkReply*>(QObject::sender());

    qDebug() << reply->readAll() << endl;
    reply->deleteLater();
    m_pNetwork->deleteLater();

    flushToken();
    emit loggedOut();
}

void ApplicationUI::flushToken() {
    m_settings.remove("access_token");
    m_settings.remove("expires_in");
    m_settings.remove("expires_time_stamp");
}

void ApplicationUI::initPageSizes(bb::cascades::DropDown* dropDown) {
    if (!dropDown) {
        return;
    }

    int amountPerRequest = AppConfig::getStatic("amount_per_request").toInt();

    QList<int> pageSizes;
    pageSizes << 10 << 15 << 20 << 25 << 30 << 35 << 40 << 45 << 50;
    foreach(int ps, pageSizes) {
        Option *option = new Option();
        option->setText(QString::number(ps));
        option->setValue(ps);

        if ((amountPerRequest == 0 && ps == PAGE_SIZE) || (amountPerRequest == ps)) {
            option->setSelected(true);
        }

        dropDown->add(option);
    }
}

int ApplicationUI::currentPageSize() const {
    int amount = AppConfig::getStatic("amount_per_request").toInt();
    return amount == 0 ? PAGE_SIZE : amount;
}
