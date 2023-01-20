#include "comdata.h"

void ComData::setComment(const QString &newComment)
{
    m_comment = newComment;
}

void ComData::printAll()
{
    qInfo()<<"Data"<<this->m_dataOrder<<" :"<<this->m_name<<this->m_qtType<<this->m_comment<<"length:"<<this->m_byteLength;
}

ComData::ComData(QObject *parent, QString name, QString rawType, int dataOrder)
{
    ComData::logDate = QDateTime::currentDateTime();  // store the last update on interface
    int exception=0;
    try {
        if (name.isEmpty()){
            exception=1;
            throw exception;
        }else{
            this->m_name = name;
        }
        if (rawType.isEmpty()){
            exception=2;
            throw exception;
        }else{
            this->m_rawType = rawType.toUpper().trimmed();
        }

        if ((this->m_rawType == "BYTE") || (this->m_rawType == "USINT")){
            this->m_qtType ="qInt8";
            this->m_byteLength= 1;
            this->m_byteOrder=ComData::s_byteIndex;
            this->m_dataOrder= (dataOrder != -1)? dataOrder:(s_totalDataCount+1) ; // take given data order if It is not given, predict from static datacounter
            ComData::s_totalDataCount++;
            ComData::s_byteIndex+= this->m_byteLength;
        }else if((this->m_rawType=="WORD")||(this->m_rawType=="UINT")) {
            this->m_qtType ="quInt16";
            this->m_byteLength= 2;
            this->m_byteOrder=ComData::s_byteIndex;
            this->m_dataOrder= (dataOrder != -1)? dataOrder:(s_totalDataCount+1) ;  // take given data order if It is not given, predict from static datacounter
            ComData::s_totalDataCount++;
            ComData::s_byteIndex+= this->m_byteLength;
        }else if((this->m_rawType=="DWORD")||(this->m_rawType=="UDINT")) {
            this->m_qtType ="quInt32";
            this->m_byteLength= 4;
            this->m_byteOrder=ComData::s_byteIndex;
            this->m_dataOrder= (dataOrder != -1)? dataOrder:(s_totalDataCount+1) ;  // take given data order if It is not given, predict from static datacounter
            ComData::s_totalDataCount++;
            ComData::s_byteIndex+= this->m_byteLength;
        }else if(this->m_rawType=="DINT") {
            this->m_qtType ="qInt32";
            this->m_byteLength= 4;
            this->m_byteOrder=ComData::s_byteIndex;
            this->m_dataOrder= (dataOrder != -1)? dataOrder:(s_totalDataCount+1) ;  // take given data order if It is not given, predict from static datacounter
            ComData::s_totalDataCount++;
            ComData::s_byteIndex+= this->m_byteLength;
        }else if(this->m_rawType=="SINT") {
            this->m_qtType ="qInt8";
            this->m_byteLength= 1;
            this->m_byteOrder=ComData::s_byteIndex;
            this->m_dataOrder= (dataOrder != -1)? dataOrder:(s_totalDataCount+1) ;  // take given data order if It is not given, predict from static datacounter
            ComData::s_totalDataCount++;
            ComData::s_byteIndex+= this->m_byteLength;
        }else if(this->m_rawType=="INT") {
            this->m_qtType ="qInt16";
            this->m_byteLength= 2;
            this->m_byteOrder=ComData::s_byteIndex;
            this->m_dataOrder= (dataOrder != -1)? dataOrder:(s_totalDataCount+1) ;  // take given data order if It is not given, predict from static datacounter
            ComData::s_totalDataCount++;
            ComData::s_byteIndex+= this->m_byteLength;
        }else if(this->m_rawType=="REAL") {
            this->m_qtType ="qReal";
            this->m_byteLength= 4;
            this->m_byteOrder=ComData::s_byteIndex;
            this->m_dataOrder= (dataOrder != -1)? dataOrder:(s_totalDataCount+1) ;  // take given data order if It is not given, predict from static datacounter
            ComData::s_totalDataCount++;
            ComData::s_byteIndex+= this->m_byteLength;
        }else{
                exception= 5;
                throw exception;
             }
    } catch (int exceptionid) {

               switch(exception){
               case 1:
                   qCritical() << "Data name invalid :" << this->m_rawType << " data at : " << QString::number(dataOrder) ;
                   break;
               case 2:
                   qCritical() << "Name"<< this->m_name <<"Data interface type invalid :" << this->m_rawType << " data at : " << QString::number(dataOrder) ;
                   break;
               case 5:
                   qCritical() << "Name"<< this->m_name <<"Data type invalid :" << this->m_rawType << " data at : " << QString::number(dataOrder) ;
                   break;

               default: ;
                   qCritical() << "Undefined exception" ;
                   break;
               }
    }


}

ComData::~ComData()
{
    try {
        if(ComData::s_byteIndex < this->m_byteLength){
            QString message = "After data named "+ this->m_name +"byte index corrupted";
            throw message;
        }else{
            ComData::s_byteIndex = (ComData::s_byteIndex)-(this->m_byteOrder);
            qInfo()<<"Total index number downgraded for removed data";
        }

    } catch (QString message) {
        qCritical()<<message;
    }

    try {
        if(ComData::s_totalDataCount < 1){
            QString message = "After data named "+ this->m_name + "data counter corrupted";
            throw message;
        }else{
            ComData::s_byteIndex--;
            qInfo()<<"Total data counter downgraded for removed data";
        }

    } catch (QString message) {
        qCritical()<<message;
    }
}
