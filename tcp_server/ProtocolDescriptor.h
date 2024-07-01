#ifndef PROTOCOLDESCRIPTOR_H
#define PROTOCOLDESCRIPTOR_H

#include <QDebug>

enum KEY_VALUES{
    get_ID,
    set_ID,
    get_name,
    set_name,
    get_address,
    set_address,
};

static void parseMSG(QString msg, int rez = 0) {
    qInfo() << msg;
    int key = msg.toInt();
    rez = key;
    switch(key) {
    case get_ID: {
        break;
    }

    case get_name:{
        break;
    }

    case get_address: {
        break;
    }
    default:
        qDebug() << "this thing not supported yet";
        break;
    }

}

const int header_size = 2;


#endif // PROTOCOLDESCRIPTOR_H
