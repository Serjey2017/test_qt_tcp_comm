#include "tcpserverhandler.h"
//#include <QDebug>

TCPServerHandler::TCPServerHandler(int port, QObject *parent)
{
    tcp_server = new QTcpServer();

    if(tcp_server->listen(QHostAddress::LocalHost, port) ) {
        connect(tcp_server, SIGNAL(newConnection()), this, SLOT(HandlerNewConnection()));

    } else {
        qCritical("Unable to start the server"); //what: QString("Unable to start the server: %1.").arg(tcp_server->errorString()));
        exit(EXIT_FAILURE);
    }

    qDebug("server started");
}

void TCPServerHandler::HandlerNewConnection() {
    qDebug("new client");

    QTcpSocket *socket = tcp_server->nextPendingConnection();

        socket->write("Hello client\r\n");
        socket->flush();

        sendMessage(socket);

        readMessage(socket);

        socket->waitForBytesWritten(3000);

        socket->close();

        //QThread * worker_trd = new QThread(); // ToDO: push thread for each client
        //processing_queue.emplace_back(worker_trd);
}


void TCPServerHandler::sendMessage(QTcpSocket* socket)
{
    if(socket)
    {
        if(socket->isOpen())
        {
            QString str = "test string to sent";

            QDataStream socketStream(socket);
            QByteArray byteArray = str.toUtf8();
            socketStream << byteArray;
        }
        else
            qCritical("QTCPServer: failed to open socket");
    }
    else
        qCritical("QTCPServer: No connection");
}


void TCPServerHandler::readMessage(QTcpSocket* socket)
{
    QByteArray buffer;
    QDataStream socketStream(socket);

    socketStream.startTransaction();
    socketStream >> buffer;


    QString message = QString("%1 :: %2").arg(socket->socketDescriptor()).arg(QString::fromStdString(buffer.toStdString()));
    emit newMessage(message);
}

void TCPServerHandler::newMessage(QString &msg)
{
  //ToDo: parse incoming msg here
    int rez = -1;
    parseMSG(msg, rez);
}
