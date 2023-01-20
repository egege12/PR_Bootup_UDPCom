#ifndef COMDATA_H
#define COMDATA_H

#include <QObject>
#include <QVariant>
#include <QtGlobal>
#include <QDebug>
#include <QDateTime>


class ComData : public QObject
{
    Q_OBJECT

public:
        ///Data read directly from asc document
        QString m_name;
        QString m_rawType;
        QString m_comment;
        //Direct calculations
        unsigned int m_dataOrder;
        unsigned int m_byteOrder;
        unsigned int m_byteLength;
        //Static indexes
        static unsigned int s_byteIndex;
        static unsigned int s_totalDataCount;
        static QDateTime logDate;
        //Use
        QString m_qtType;


    explicit ComData(QObject *parent = nullptr,QString name="",QString rawType="", int dataOrder=-1);
    ~ComData();

        ///  Comment setters
        void setComment(const QString &newComment);
        void printAll();
        /// Value
        QVariant value;
signals:

};

#endif // COMDATA_H
