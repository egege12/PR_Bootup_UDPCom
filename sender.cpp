// Copyright (C) 2017 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

#include <QtWidgets>
#include <QtNetwork>
#include <QtCore>
#include <array>
#include "sender.h"

using namespace std;
myUdpSocket::myUdpSocket(QObject *parent, comdatatcmsimporter * const receiveData, QString receiveIP, QString receiverPort, comdatatcmsimporter * const transmitData, QString transmitIP, QString transmitPort)
{
    this->receiveData = receiveData;
    this->transmitData = transmitData;
    this->receiverPort  = receiverPort;
    this->transmitPort = transmitPort;
    this->transmitIP = transmitIP;
    this->receiveIP  = receiveIP;
    ///Create  recevier socket
    udpReceiveSocket = new QUdpSocket(this);
    udpReceiveSocket-> bind((QHostAddress::AnyIPv4),this->receiverPort.toInt(),QUdpSocket::ShareAddress);
    udpReceiveSocket ->joinMulticastGroup(QHostAddress(this->receiveIP));
    QObject::connect(udpReceiveSocket,&QUdpSocket::readyRead,this,&myUdpSocket::readPendingDatagrams);
    ///Create transmit socket
    udpTransmitterSocket = new QUdpSocket(this);
    udpTransmitterSocket-> bind();
    ///Connect the sockets


    //Start timer
    QObject::connect(&transmitterTimer,&QTimer::timeout,this,&myUdpSocket::sendTheDatagram);



    statusLabel = new QLabel(tr("Ready to broadcast datagrams on port 17224"));
    statusLabel->setWordWrap(true);
    statusLabel2 = new QLabel(tr("Listen multicast"));
    statusLabel2->setWordWrap(true);
    startButton = new QPushButton(tr("&Start"));
    auto quitButton = new QPushButton(tr("&Quit"));

    auto buttonBox = new QDialogButtonBox;
    buttonBox->addButton(startButton, QDialogButtonBox::ActionRole);
    buttonBox->addButton(quitButton, QDialogButtonBox::RejectRole);
    connect(startButton, &QPushButton::clicked, this, &myUdpSocket::startBroadcasting);
    connect(quitButton, &QPushButton::clicked, this, &myUdpSocket::close);
    auto mainLayout = new QVBoxLayout;
    mainLayout->addWidget(statusLabel);
    mainLayout->addWidget(statusLabel2);
    mainLayout->addWidget(buttonBox);
    setLayout(mainLayout);

    setWindowTitle(tr("Broadcast Sender"));


}


void myUdpSocket::startBroadcasting()
{
    startButton->setEnabled(false);
    transmitterTimer.start(100);
}

void myUdpSocket::sendTheDatagram()
{
    statusLabel->setText(tr("Now broadcasting datagram %1").arg(messageNo));
    QByteArray odata;
        foreach(int key, transmitData->ioData.keys()){
            ComData* k_comData= transmitData->ioData[key];
            odata.append(k_comData->value.toByteArray());
            k_comData->printAll();
        }

    udpTransmitterSocket ->writeDatagram(odata,QHostAddress(this->transmitIP),this->transmitPort.toInt());
    qInfo()<<"Data sent";
    ++messageNo;

}

void myUdpSocket::readPendingDatagrams()
{
    while (udpReceiveSocket->hasPendingDatagrams()) {
            statusLabel2->setText(tr("Datagram received"));
            QNetworkDatagram datagram = udpReceiveSocket->receiveDatagram();
            processTheDatagram(&datagram);
        }
    qInfo()<<"Data received";

}

void myUdpSocket::processTheDatagram(QNetworkDatagram *datagram)
{
    QByteArray packData = datagram->data();
    foreach(int key, receiveData->ioData.keys()){
        for(qsizetype a=(receiveData->ioData[key]->m_byteOrder);a<((receiveData->ioData[key]->m_byteOrder)+ (receiveData->ioData[key]->m_byteLength));a++){
            receiveData->ioData[key]->value=packData.at(a);
            receiveData->ioData[key]->printAll();
        }
    }

}

