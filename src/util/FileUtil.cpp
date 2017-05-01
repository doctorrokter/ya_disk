/*
 * FileUtil.cpp
 *
 *  Created on: Apr 29, 2017
 *      Author: misha
 */

#include <src/util/FileUtil.hpp>

FileUtil::FileUtil(QObject* parent) : QObject(parent) {
    m_imagesList << "jpg" << "jpeg" << "gif" << "png";
    m_videoList << "3gp" << "3g2" << "asf" << "avi" << "f4v" << "ismv" << "m4v" << "mkv" << "mov" << "mp4" << "mpeg" << "wmv";
    m_audioList << "aac" << "amr" << "flac" << "mp3" << "m4a" << "wma" << "wav";
    m_docList << "doc" << "dot" << "txt" << "docx" << "dotx" << "docm" << "dotm" << "json" << "xml" << "js";
    m_xlsList << "xls" << "xlt" << "xlsx" << "xltx" << "xlsm" << "xltm" << "csv";
    m_pptList << "ppt" << "pot" << "pps" << "pptx" << "potx" << "ppsx" << "pptm" << "potm" << "ppsm";
}

FileUtil::~FileUtil() {}

bool FileUtil::isImage(const QString& ext) {
    foreach(QString e, m_imagesList) {
        if (e.compare(ext) == 0) {
            return true;
        }
    }
    return false;
}

bool FileUtil::isVideo(const QString& ext) {
    foreach(QString e, m_videoList) {
        if (e.compare(ext) == 0) {
            return true;
        }
    }
    return false;
}

bool FileUtil::isAudio(const QString& ext) {
    foreach(QString e, m_audioList) {
        if (e.compare(ext) == 0) {
            return true;
        }
    }
    return false;
}

bool FileUtil::isDoc(const QString& ext) {
    foreach(QString e, m_docList) {
        if (e.compare(ext) == 0) {
            return true;
        }
    }
    return false;
}

bool FileUtil::isSpreadSheet(const QString& ext) {
    foreach(QString e, m_xlsList) {
        if (e.compare(ext) == 0) {
            return true;
        }
    }
    return false;
}

bool FileUtil::isPresentation(const QString& ext) {
    foreach(QString e, m_pptList) {
        if (e.compare(ext) == 0) {
            return true;
        }
    }
    return false;
}

bool FileUtil::isPdf(const QString& ext) {
    return ext.compare("pdf") == 0;
}

QString FileUtil::filename(const QString& filepath) {
    QStringList parts = filepath.split("/");
    return parts[parts.size() - 1];
}

