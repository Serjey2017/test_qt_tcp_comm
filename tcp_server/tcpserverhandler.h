#ifndef TCPSERVERHANDLER_H
#define TCPSERVERHANDLER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
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
    void readSocket();

private:
    unsigned int port = -1;
    QTcpServer *tcp_server;
    QStack<QString> client_list;

    // for test reason
    const int test_ID = 15;
    const QString test_name = "Valera";
    const QString test_address = "Oslo";
};

#endif // TCPSERVERHANDLER_H
