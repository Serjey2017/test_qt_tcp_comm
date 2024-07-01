#include "tcpserverhandler.h"
//#include <QDebug>
#include <QPair>

TCPServerHandler::TCPServerHandler(int port, QObject *parent)
{
    tcp_server = new QTcpServer();

    if(tcp_server->listen(QHostAddress::LocalHost, port) ) {
        connect(tcp_server, SIGNAL(newConnection()), this, SLOT(HandlerNewConnection()));

    } else {
        qCritical() << "Unable to start the server" << tcp_server->errorString();
        exit(EXIT_FAILURE);
    }

    qDebug("server started");
}

void TCPServerHandler::HandlerNewConnection() {
    qDebug("new client");

    QTcpSocket *socket = tcp_server->nextPendingConnection();
    connect(socket, &QTcpSocket::readyRead, this, &TCPServerHandler::readSocket);
}

void TCPServerHandler::readSocket() {
    QTcpSocket* socket = reinterpret_cast<QTcpSocket*>(sender());
    readMessage(socket);
}

static void sendMsg(QTcpSocket *socket, QString request, const int header_size) {
    if(socket)
        {
            if(socket->isOpen())
            {
                QDataStream socketStream(socket);

                QByteArray header;
                header.prepend(QString("fileType:message,msgSize:%1;").arg(request.size()).toUtf8());
                header.resize(header_size);

                QByteArray byteArray = request.toUtf8();
                byteArray.prepend(header);

                socketStream << byteArray;

            }
            else
                qCritical("TCPClient: failed to open socket");
        }
        else
            qCritical("TCPClient: no connection");

}


void TCPServerHandler::sendMessage(QTcpSocket* socket)
{
//    if(socket)
//    {
//        if(socket->isOpen())
//        {
//            QString str = "test string to sent";

//            QDataStream socketStream(socket);
//            QByteArray byteArray = str.toUtf8();
//            socketStream << byteArray;
//        }
//        else
//            qCritical("QTCPServer: failed to open socket");
//    }
//    else
//        qCritical("QTCPServer: No connection");
    sendMsg(socket, QString::number(KEY_VALUES::set_name), header_size);
}


void TCPServerHandler::readMessage(QTcpSocket* socket)
{
    QByteArray buffer;
    QDataStream socketStream(socket);

    socketStream.startTransaction();
    socketStream >> buffer;


    if(!socketStream.commitTransaction())
    {
        QString message = QString("%1 :: Waiting for more data to come..").arg(socket->socketDescriptor());
        emit newMessage(message);
        return;
    }

    QString header = buffer.mid(0, header_size);
    buffer = buffer.mid(header_size);

    //QString message = QString("%1 :: %2").arg(socket->socketDescriptor()).arg(QString::fromStdString(buffer.toStdString()));
    QString message = buffer;

//    QPair<QString, QTcpSocket> pair;
//    pair.first = buffer;

//    emit newMessage(pair); //message);
    emit newMessage(message);
}

void TCPServerHandler::newMessage(QString &msg)
//void TCPServerHandler::newMessage(QPair<QString, QTcpSocket> pair)
{
    int rez = -1;
    parseMSG(msg, rez);
//    parseMSG(pair.first, rez);
//    if(rez == get_ID) {
//        sendMsg(&pair.second, QString::number(KEY_VALUES::set_name), header_size);
//    }
}
