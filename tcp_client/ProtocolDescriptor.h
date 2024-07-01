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
    qDebug() << "parse msg here";
    qInfo() << msg;
}

const QString custom_header = "0F";
const int header_size = 2;

/// msg: header(id, name, address) + body(payload)

#endif // PROTOCOLDESCRIPTOR_H
