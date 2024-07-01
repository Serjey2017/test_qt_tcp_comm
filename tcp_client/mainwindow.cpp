#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "ProtocolDescriptor.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    socket = new QTcpSocket(this);

    connect(this, &MainWindow::newMessage, this, &MainWindow::displayMessage);
    connect(socket, &QTcpSocket::readyRead, this, &MainWindow::readSocket);
    connect(socket, &QAbstractSocket::errorOccurred, this, &MainWindow::displayError);


    socket->connectToHost(default_addr, default_port);
    socket->waitForConnected(5000);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_lineEdit_editingFinished()
{
  // reset connection
    if(socket->isOpen()) {
        socket->flush();
        socket->disconnect();
        socket->reset();

        socket->connectToHost(QHostAddress(ui->lineEdit->text()), default_port);
        socket->waitForConnected(5000);

    } else {
        socket->connectToHost(QHostAddress(ui->lineEdit->text()), default_port);
        socket->waitForConnected(5000);

    }
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

void MainWindow::on_pb_getID_clicked()
{
  // request id from server
    sendMsg(socket, QString::number(KEY_VALUES::get_ID), header_size);
}


void MainWindow::on_pb_getname_clicked()
{
    // request name
    sendMsg(socket, QString::number(KEY_VALUES::get_name), header_size);
}


void MainWindow::on_pb_getaddress_clicked()
{
    // request addr
    sendMsg(socket, QString::number(KEY_VALUES::get_address), header_size);
}

void MainWindow::readSocket()
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

    QString message = QString("%1 :: %2").arg(socket->socketDescriptor()).arg(QString::fromStdString(buffer.toStdString()));
    emit newMessage(message);
}

void MainWindow::displayMessage(const QString& str)
{
    ui->pb_getID->setText(str);
}

void MainWindow::displayError() {
    qCritical() << "failed to attach to server";
}
