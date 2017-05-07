/*
 * DateUtil.cpp
 *
 *  Created on: May 7, 2017
 *      Author: misha
 */

#include <src/util/DateUtil.hpp>
#include "../Common.hpp"
#include "../config/AppConfig.hpp"

DateUtil::DateUtil(QObject* parent) : QObject(parent) {}

DateUtil::~DateUtil() {}

QString DateUtil::str(const QDateTime& dateTime) {
    QDateTime dt = dateTime;
    QString df = AppConfig::getStatic("date_format").toString();
    if (df.isEmpty() || df.compare(DATE_FORMAT) == 0) {
        return dt.toString(DATE_FORMAT);
    }
    return dt.toString(m_locale.dateTimeFormat(QLocale::ShortFormat));
}
