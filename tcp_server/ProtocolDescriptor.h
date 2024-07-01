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


}

const int header_size = 2;


#endif // PROTOCOLDESCRIPTOR_H
