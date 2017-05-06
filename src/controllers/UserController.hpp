/*
 * UserController.hpp
 *
 *  Created on: May 5, 2017
 *      Author: misha
 */

#ifndef USERCONTROLLER_HPP_
#define USERCONTROLLER_HPP_

#include <QObject>
#include <QVariantMap>
#include "../webdav/qwebdav.h"
#include "../webdav/qwebdavdirparser.h"
#include "../models/User.hpp"

class UserController: public QObject {
    Q_OBJECT
    Q_PROPERTY(User* user READ getUser NOTIFY userChanged)
public:
    UserController(QObject* parent = 0);
    virtual ~UserController();

    Q_INVOKABLE void userinfo();
    Q_INVOKABLE void diskinfo();

    Q_INVOKABLE User* getUser() const;

    void initWebdav(QWebdav* webdav, QWebdavDirParser* parser);

Q_SIGNALS:
    void userinfoChanged(const QString& login);
    void userChanged(QVariantMap userMap);

private slots:
    void onUserinfoLoaded();
    void onDiskinfoLoaded();

private:
    QWebdav* m_pWebdav;
    QWebdavDirParser* m_pParser;

    User* m_pUser;
};

#endif /* USERCONTROLLER_HPP_ */
