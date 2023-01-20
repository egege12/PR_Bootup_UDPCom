// Copyright (C) 2017 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

#ifndef SENDER_H
#define SENDER_H

#include <QWidget>
#include <QTimer>
#include "comdatatcmsimporter.h"
#include <QNetworkDatagram>
QT_BEGIN_NAMESPACE
class QLabel;
class QPushButton;
class QUdpSocket;
QT_END_NAMESPACE

class myUdpSocket : public QWidget
{
    Q_OBJECT

public:
    explicit myUdpSocket(QObject *parent = nullptr,comdatatcmsimporter* const receiveData = nullptr,QString receiveIP = "0", QString receiverPort = "0",comdatatcmsimporter* const transmitData = nullptr,QString transmitIP="0", QString transmitPort = "0");
    void processTheDatagram(QNetworkDatagram* datagram);
public slots:
    void startBroadcasting();
    void sendTheDatagram();
    void readPendingDatagrams();
private:
    QLabel *statusLabel = nullptr;
    QLabel *statusLabel2 = nullptr;
    QPushButton *startButton = nullptr;
    QUdpSocket *udpSocket = nullptr;
    QUdpSocket *udpSocket2 = nullptr;
    QUdpSocket *udpTransmitterSocket;
    QUdpSocket *udpReceiveSocket;
    QString transmitPort;
    QString receiverPort;
    QString receiveIP;
    QString transmitIP;
    comdatatcmsimporter* receiveData;
    comdatatcmsimporter* transmitData;
    QTimer transmitterTimer;
    int messageNo = 1;
};

#endif
