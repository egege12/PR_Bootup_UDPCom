#ifndef UDPSOCKET_H
#define UDPSOCKET_H

#include <QObject>

class udpsocket : public QObject
{
    Q_OBJECT
public:
    explicit udpsocket(QObject *parent = nullptr);

signals:

};

#endif // UDPSOCKET_H
