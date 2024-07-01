#ifndef TCPSERVERHANDLER_H
#define TCPSERVERHANDLER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QThread>
#include <QSemaphore>
#include <QVector>
#include <QStack>

#include "ProtocolDescriptor.h"


class TCPServerHandler : public QObject
{
    Q_OBJECT
public:
    TCPServerHandler() { TCPServerHandler(5000); qDebug("created tcp server with default parameters"); };
    TCPServerHandler(int port, QObject *parent = 0);

    void sendMessage(QTcpSocket* socket);
    void readMessage(QTcpSocket* socket);

    void newMessage(QString &msg);
    void MessageToSent(QString &msg);

public slots:
    void HandlerNewConnection();

private:
    unsigned int port = -1;
    bool _should_work = 0;
    QTcpServer *tcp_server;
    QStack<QString> client_list;
    QVector<QThread> processing_queue;
};

#endif // TCPSERVERHANDLER_H
