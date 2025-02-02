#include "tcpserverhandler.h"
#include <QPair>
#include "../TestData.h"

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
                header.prepend(QString("%1%2;").arg(custom_header.toUtf8()).arg(request.size()).toUtf8());
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
    sendMsg(socket, QString::number(KEY_VALUES::set_name), header_size);
}


void TCPServerHandler::readMessage(QTcpSocket* socket)
{
    QByteArray buffer;
    QPair<QString, QTcpSocket*> pair;
    QDataStream socketStream(socket);

    socketStream.startTransaction();
    socketStream >> buffer;


    if(!socketStream.commitTransaction())
    {
        pair.first = QString("%1 :: Waiting for more data to come..").arg(socket->socketDescriptor());
        pair.second = socket;
        emit newMessage(pair);
        return;
    }

    QString header = buffer.mid(0, header_size);
    buffer = buffer.mid(header_size);

    pair.first = buffer;
    pair.second = socket;

    emit newMessage(pair);
}

void TCPServerHandler::newMessage(QPair<QString, QTcpSocket*> pair)
{
    // ugly conditioning
    int key = pair.first.toInt();
    switch(key) {
    case get_ID: {
        QString test_ID = QString::number( QRandomGenerator::global()->bounded(256));
        sendMsg(pair.second, QString::number(KEY_VALUES::set_ID) + test_ID, header_size);
        break;
    }

    case get_name:{
        QString test_name = NAME_VALUES.at(QRandomGenerator::global()->bounded(NAME_VALUES.size()));
        sendMsg(pair.second, QString::number(KEY_VALUES::set_name) + test_name, header_size);
        break;
    }

    case get_address: {
        QString test_address = ADDRESS_VALUES.at(QRandomGenerator::global()->bounded(ADDRESS_VALUES.size()));
        sendMsg(pair.second, QString::number(KEY_VALUES::set_address) + test_address, header_size);
        break;
    }
    default:
        qDebug() << "hold on, this thing not supported yet";
        break;
    }
}
