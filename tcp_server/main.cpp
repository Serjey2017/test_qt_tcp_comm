#include <QCoreApplication>
#include "tcpserverhandler.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    TCPServerHandler *tcp_handler;
    tcp_handler = new TCPServerHandler(5000);


    return a.exec();
}
