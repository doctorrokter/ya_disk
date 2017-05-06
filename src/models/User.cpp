/*
 * User.cpp
 *
 *  Created on: May 5, 2017
 *      Author: misha
 */

#include <src/models/User.hpp>

User::User(QObject* parent) : QObject(parent), m_uid(0), m_login(""), m_fio(""),
    m_firstname(""), m_lastname(""), m_uploadConcurrency(0), m_datasyncDbPrefix(""), m_isB2b(false),
    m_availableBytes(0), m_usedBytes(0) {}

User::~User() {}

int User::getUid() const { return m_uid; }
void User::setUid(const int& uid) {
    m_uid = uid;
    emit uidChanged(m_uid);
}

const QString& User::getLogin() const { return m_login; }
void User::setLogin(const QString& login) {
    m_login = login;
    emit loginChanged(m_login);
}

const QString& User::getFio() const { return m_fio; }
void User::setFio(const QString& fio) {
    m_fio = fio;
    emit fioChanged(m_fio);
}

const QString& User::getFirstname() const { return m_firstname; }
void User::setFirstname(const QString& firstname) {
    m_firstname = firstname;
    emit firstnameChanged(m_firstname);
}

const QString& User::getLastname() const { return m_lastname; }
void User::setLastname(const QString& lastname) {
    m_lastname = lastname;
    emit lastnameChanged(m_lastname);
}

int User::getUploadConcurrency() const { return m_uploadConcurrency; }
void User::setUploadConcurrency(const int& uploadConcurrency) {
   m_uploadConcurrency = uploadConcurrency;
   emit uploadConcurrencyChanged(m_uploadConcurrency);
}

const QString& User::getDatasyncDbPrefix() const { return m_datasyncDbPrefix; }
void User::setDatasyncDbPrefix(const QString& datasyncDbPrefix) {
    m_datasyncDbPrefix = datasyncDbPrefix;
    emit datasyncDbPrefixChanged(m_datasyncDbPrefix);
}

bool User::isB2b() const { return m_isB2b; }
void User::setB2b(const bool& isB2b) {
    m_isB2b = isB2b;
    emit b2bChanged(m_isB2b);
}

long long User::getAvailableBytes() const { return m_availableBytes; }
void User::setAvailableBytes(const long long& availableBytes) {
    m_availableBytes = availableBytes;
    emit availableBytesChanged(m_availableBytes);
}

long long User::getUsedBytes() const { return m_usedBytes; }
void User::setUsedBytes(const long long& usedBytes) {
    m_usedBytes = usedBytes;
    emit usedBytesChanged(m_usedBytes);
}

QVariantMap User::toMap() const {
    QVariantMap userMap;
    userMap["uid"] = this->getUid();
    userMap["login"] = this->getLogin();
    userMap["fio"] = this->getFio();
    userMap["firstname"] = this->getFirstname();
    userMap["lastname"] = this->getLastname();
    userMap["available_bytes"] = this->getAvailableBytes();
    userMap["used_bytes"] = this->getUsedBytes();
    return userMap;
}
