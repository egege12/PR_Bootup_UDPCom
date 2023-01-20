#ifndef COMDATATCMSIMPORTER_H
#define COMDATATCMSIMPORTER_H

#include <QObject>
#include <QMap>
#include "comdata.h"
#include <QFile>
#include <QDebug>
#include <QFile>
typedef QMap<int,ComData*> comData;

class comdatatcmsimporter : public QObject
{

    Q_OBJECT
public:
    comData ioData;
    void createInterface(QObject *parent= nullptr, QString _path = "../")
    {
        QFile ascFile(_path);
        bool readFlag =false;
        bool fileVerified=false;
        bool fileCantOpen=false;
        bool stopFlag= false;
        bool startCommentRelease = false;
        int dataCounter=0;
        QString oldName = "";
        int oldCount = 0;

        try {
            if (!ascFile.open(QIODevice::ReadOnly | QIODevice::Text)){
                fileCantOpen=true;
            }else{
                QTextStream in(&ascFile);
                while (!in.atEnd()) {
                    QString line = in.readLine();
                    line=line.remove("\t");
                    if (fileVerified){
                        if (readFlag && !stopFlag){
                            if(line.contains("END_STRUCT;") || line.contains("bEnd")){
                                stopFlag=true;
                                break;
                        }
                            bool expectComment=false;
                            if(line.contains(":=")){
                                dataCounter++;
                                oldName = readName(line);
                                oldCount = dataCounter;
                                ComData *p_iodata = new ComData(parent,oldName,readType(line),dataCounter);
                                ioData.insert(dataCounter,p_iodata);
                                startCommentRelease = true;
                            }else if(line.contains("(*") && startCommentRelease){
                                ioData[oldCount]->setComment(readComment(line));
                            }


                        }else{
                            if (line.contains("wStart:") || line.contains("dwStart:")){
                                readFlag=true;
                            }
                        }
                    }else{
                        if (line.contains("DUTs")){
                            fileVerified=true;
                        }
                    }

                }
            }
            if (fileCantOpen){
                QString message = "File can not be found";
                throw message;
            }else if (!fileVerified){
                QString message = "File not verified, please use DUT export";
                throw message;
            }else if(!readFlag){
                QString message = "wStart identifier can not be found please add on DUT start data \"wStart\" as WORD and to the  end \"bend\" as BYTE";
                throw message;
            }

        } catch (QString message) {

            qInfo()<<message;

        }
    }
    void printInterfaceConsole()
    {
        foreach(int key, ioData.keys()){
            ComData* k_comData= ioData[key];
            k_comData->printAll();
        }
    }

    QString readName(QString& line)
    {
        QString m_name="";
        qsizetype index ;
        m_name = line.trimmed();
        index= m_name.indexOf(":");
        return line.mid(0,index);
    }
    QString readType(QString& line)
    {
        QString typeName=line;
        qsizetype contindex=typeName.indexOf(": ");
        typeName=typeName.remove(0,contindex);
        contindex=((typeName.indexOf(":=")-2));
        typeName=typeName.mid(2,contindex);
        return typeName;
    }
    QString readComment(QString& line)
    {
        QString comment;

        if (line.indexOf("(*")!= -1){
            comment= line.trimmed();
            comment= line.replace("(*","/*");
            comment= line.replace("*)","*/");
        }else
            return (QString)"No comment detected at subfunction";

        return comment;
    }
    bool createHeader(QString interfaceHeaderName){

        QFile file(interfaceHeaderName);
            if (!file.open(QIODevice::ReadWrite | QIODevice::Text))
                return false;

        QTextStream out(&file);
               out << "The magic number is: " << 49 << "\n";
            return true;
    }

signals:

};

#endif // COMDATATCMSIMPORTER_H
