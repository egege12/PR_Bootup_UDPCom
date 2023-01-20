// Copyright (C) 2016 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

#include <QApplication>
#include "sender.h"
#include "comdata.h"
#include "comdatatcmsimporter.h"
unsigned int ComData::s_byteIndex = 0;
unsigned int ComData::s_totalDataCount = 0;
QDateTime ComData::logDate = QDateTime::currentDateTime();
int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    comdatatcmsimporter idata,odata;

    idata.createInterface(&app,"C:/Users/egementurker/Desktop/DUTs/tim_sw_idata.asc");
    idata.printInterfaceConsole();
    odata.createInterface(&app,"C:/Users/egementurker/Desktop/DUTs/tim_sw_odata.asc");
    odata.printInterfaceConsole();
    myUdpSocket portUDP(&app,&idata,"230.0.0.100","17325",&odata,"192.168.101.1","17224");

    portUDP.show();
    return app.exec();
}
