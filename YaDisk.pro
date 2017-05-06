APP_NAME = YaDisk

CONFIG += qt warn_on cascades10

LIBS += -lbbnetwork -lbbdevice -lbbsystem -lbbcascadespickers -lbbpim -lbb -lbbdata -lbbplatform

QT += network xml

DEFINES += DEBUG_WEBDAV QWEBDAVITEM_EXTENDED_PROPERTIES

include(config.pri)
