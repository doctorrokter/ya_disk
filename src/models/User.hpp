/*
 * User.hpp
 *
 *  Created on: May 5, 2017
 *      Author: misha
 */

#ifndef USER_HPP_
#define USER_HPP_

#include <QObject>
#include <QVariantMap>

class User: public QObject {
    Q_OBJECT
    Q_PROPERTY(int uid READ getUid WRITE setUid NOTIFY uidChanged)
    Q_PROPERTY(QString login READ getLogin WRITE setLogin NOTIFY loginChanged)
    Q_PROPERTY(QString fio READ getFio WRITE setFio NOTIFY fioChanged)
    Q_PROPERTY(QString firstname READ getFirstname WRITE setFirstname NOTIFY firstnameChanged)
    Q_PROPERTY(QString lastname READ getLastname WRITE setLastname NOTIFY lastnameChanged)
    Q_PROPERTY(int uploadConcurrency READ getUploadConcurrency WRITE setUploadConcurrency NOTIFY uploadConcurrencyChanged)
    Q_PROPERTY(QString datasyncDbPrefix READ getDatasyncDbPrefix WRITE setDatasyncDbPrefix NOTIFY datasyncDbPrefixChanged)
    Q_PROPERTY(bool isB2b READ isB2b WRITE setB2b NOTIFY b2bChanged)
    Q_PROPERTY(long long availableBytes READ getAvailableBytes WRITE setAvailableBytes NOTIFY availableBytesChanged)
    Q_PROPERTY(long long usedBytes READ getUsedBytes WRITE setUsedBytes NOTIFY usedBytesChanged)
public:
    User(QObject* parent = 0);
    virtual ~User();

    Q_INVOKABLE int getUid() const;
    Q_INVOKABLE void setUid(const int& uid);

    Q_INVOKABLE const QString& getLogin() const;
    Q_INVOKABLE void setLogin(const QString& login);

    Q_INVOKABLE const QString& getFio() const;
    Q_INVOKABLE void setFio(const QString& fio);

    Q_INVOKABLE const QString& getFirstname() const;
    Q_INVOKABLE void setFirstname(const QString& firstname);

    Q_INVOKABLE const QString& getLastname() const;
    Q_INVOKABLE void setLastname(const QString& lastname);

    Q_INVOKABLE int getUploadConcurrency() const;
    Q_INVOKABLE void setUploadConcurrency(const int& uploadConcurrency);

    Q_INVOKABLE const QString& getDatasyncDbPrefix() const;
    Q_INVOKABLE void setDatasyncDbPrefix(const QString& datasyncDbPrefix);

    Q_INVOKABLE bool isB2b() const;
    Q_INVOKABLE void setB2b(const bool& isB2b);

    Q_INVOKABLE long long getAvailableBytes() const;
    Q_INVOKABLE void setAvailableBytes(const long long& availableBytes);

    Q_INVOKABLE long long getUsedBytes() const;
    Q_INVOKABLE void setUsedBytes(const long long& usedBytes);

    QVariantMap toMap() const;
Q_SIGNALS:
    void uidChanged(const int& uid);
    void loginChanged(const QString& login);
    void fioChanged(const QString& fio);
    void firstnameChanged(const QString& firstname);
    void lastnameChanged(const QString& lastname);
    void uploadConcurrencyChanged(const int& uploadConcurrency);
    void datasyncDbPrefixChanged(const QString& datasyncDbPrefix);
    void b2bChanged(const bool& isB2b);
    void availableBytesChanged(const long long& availableBytes);
    void usedBytesChanged(const long long& usedBytes);

private:
    int m_uid;
    QString m_login;
    QString m_fio;
    QString m_firstname;
    QString m_lastname;
    int m_uploadConcurrency;
    QString m_datasyncDbPrefix;
    bool m_isB2b;
    long long m_availableBytes;
    long long m_usedBytes;
};

#endif /* USER_HPP_ */
