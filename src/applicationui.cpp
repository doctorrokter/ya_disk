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
#include <bb/data/JsonDataAccess>

using namespace bb::cascades;
using namespace bb::data;

ApplicationUI::ApplicationUI() : QObject() {

    QCoreApplication::setOrganizationName("mikhail.chachkouski");
    QCoreApplication::setApplicationName("YaDisk");

    QDir dataDir(QDir::currentPath() + TEMP_DIR);
    dataDir.setNameFilters(QStringList() << "*.*");
    dataDir.setFilter(QDir::Files);
    foreach(QString dirFile, dataDir.entryList()) {
        qDebug() << "ApplicationUI ===>>> remove temp file: " << dirFile << endl;
        dataDir.remove(dirFile);
    }

    m_pAppConfig = new AppConfig(this);
    m_pTranslator = new QTranslator(this);
    m_pLocaleHandler = new LocaleHandler(this);
    m_pFileUtil = new FileUtil(this);
    m_pFileController = new FileController(m_pFileUtil, this);
    m_pUserController = new UserController(this);

    bool res = QObject::connect(m_pLocaleHandler, SIGNAL(systemLanguageChanged()), this, SLOT(onSystemLanguageChanged()));
    Q_ASSERT(res);
    Q_UNUSED(res);

    onSystemLanguageChanged();

    m_pAppConfig = new AppConfig(this);
    QString theme = m_pAppConfig->get("theme").toString();
    if (theme.compare("") != 0) {
        if (theme.compare("DARK") == 0) {
            Application::instance()->themeSupport()->setVisualStyle(VisualStyle::Dark);
        } else {
            Application::instance()->themeSupport()->setVisualStyle(VisualStyle::Bright);
        }
    }

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

//ApplicationUI::~ApplicationUI() {
//    m_pAppConfig->deleteLater();
//    m_pFileController->deleteLater();
//    m_pFileUtil->deleteLater();
//    m_pUserController->deleteLater();
//    m_pInvokeManager->deleteLater();
//    m_pLocaleHandler->deleteLater();
//    m_pParser->deleteLater();
//    m_pTranslator->deleteLater();
//    m_pWebdav->deleteLater();
//}

void ApplicationUI::onSystemLanguageChanged() {
    QCoreApplication::instance()->removeTranslator(m_pTranslator);
    QString locale_string = QLocale().name();
    QString file_name = QString("YaDisk_%1").arg(locale_string);
    if (m_pTranslator->load(file_name, "app/native/qm")) {
        QCoreApplication::instance()->installTranslator(m_pTranslator);
    }
}

void ApplicationUI::setToken(const QString& token, const int& expiresIn) {
    m_settings.setValue("access_token", token);
    m_settings.setValue("expires_in", expiresIn);
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
    QmlDocument *qml = QmlDocument::create("asset:///main.qml").parent(this);
    QDeclarativeEngine* engine = QmlDocument::defaultDeclarativeEngine();
    QDeclarativeContext* rootContext = engine->rootContext();
    rootContext->setContextProperty("_app", this);
    rootContext->setContextProperty("_appConfig", m_pAppConfig);
    rootContext->setContextProperty("_file", m_pFileUtil);
    rootContext->setContextProperty("_fileController", m_pFileController);
    rootContext->setContextProperty("_userController", m_pUserController);
    rootContext->setContextProperty("_data", data);
    rootContext->setContextProperty("_mimeType", mimeType);

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
