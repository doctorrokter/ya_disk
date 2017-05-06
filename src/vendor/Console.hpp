/*
 * Console.h
 *
 *  Created on: Feb 8, 2017
 *      Author: misha
 */

#ifndef CONSOLE_HPP_
#define CONSOLE_HPP_

#include <QObject>
#include <QtNetwork/QUdpSocket>

#define CLIENT_SENDING_PORT 10465

class Console : public QObject {
    Q_OBJECT
public:
    Console();
    virtual ~Console();

    void sendMessage(QString _command);

private:
    QUdpSocket *m_socket;
};

#endif /* CONSOLE_HPP_ */
