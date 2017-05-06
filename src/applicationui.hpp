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

#ifndef ApplicationUI_HPP_
#define ApplicationUI_HPP_

#include <QObject>
#include <QSettings>
#include "webdav/qwebdav.h"
#include "webdav/qwebdavdirparser.h"
#include "Common.hpp"
#include "util/FileUtil.hpp"
#include "controllers/FileController.hpp"
#include "controllers/UserController.hpp"
#include "config/AppConfig.hpp"
#include <bb/system/InvokeManager>
#include <bb/system/InvokeRequest>

using namespace bb::system;

namespace bb
{
    namespace cascades
    {
        class LocaleHandler;
    }
}

class QTranslator;

/*!
 * @brief Application UI object
 *
 * Use this object to create and init app UI, to create context objects, to register the new meta types etc.
 */
class ApplicationUI : public QObject {
    Q_OBJECT
public:
    ApplicationUI();
    virtual ~ApplicationUI() {}

    Q_INVOKABLE void setToken(const QString& token, const int& expiresIn);
    Q_INVOKABLE bool hasToken();
    Q_INVOKABLE void initWebdav();

Q_SIGNALS:
    void cardDone();

private slots:
    void onSystemLanguageChanged();
    void onCardDone(const QString& msg);
    void onInvoked(const bb::system::InvokeRequest& request);
private:
    QTranslator* m_pTranslator;
    bb::cascades::LocaleHandler* m_pLocaleHandler;

    QSettings m_settings;

    QWebdav* m_pWebdav;
    QWebdavDirParser* m_pParser;

    FileUtil* m_pFileUtil;
    FileController* m_pFileController;
    UserController* m_pUserController;
    AppConfig* m_pAppConfig;
    InvokeManager* m_pInvokeManager;
    QString m_startupMode;

    void initFullUI(const QString& data = "", const QString& mimeType = "");
};

#endif /* ApplicationUI_HPP_ */
