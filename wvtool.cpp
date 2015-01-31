#include "wvtool.h"
#include "wvtwordlist.h"
#include "wvtfileinputlist.h"
#include "wvtdocumentinfo.h"
#include "wvtwordvector.h"
#include <QFile>
#include <QTextStream>
#include <QLibrary>
#include <QChar>
#include "ICTCLAS30.h"
#include <stdio.h>
#include <stdlib.h>
#include <String>
#include <string.h>
#include "tfidf.h"
#include <iostream>
#include <QStringList>
using namespace std;

QVector<QString>* WVTool::STOPWORDS =  new QVector<QString>();
WVTool::WVTool()
{
   if(STOPWORDS->size()==0)
   {
       QFile file("data/stopwords.txt");

               if(file.open(QIODevice::ReadOnly | QIODevice::Text))
               {
                   while(!file.atEnd())
                   {
                       QByteArray bytea = file.readLine();
                       QString str(QObject::tr(QString(bytea).toStdString().c_str()));
                       //std::cout << str.toStdString();
                       STOPWORDS->append(str.trimmed());
                   }
               }


               file.flush();
               file.close();
   }
}
 WVTWordList* WVTool::createWordList(WVTFileInputList* wvtinputlist)
 {
     WVTWordList* wvtwordlist = new WVTWordList(wvtinputlist->getNumClasses());
     wvtwordlist->setAppendWords(true);
     wvtwordlist->setUpdateOnlyCurrent(false);
     wvtinputlist->getEntries();
     for(int i = 0;i < wvtinputlist->fileList.size();i++)
     {
         WVTDocumentInfo* wdif = wvtinputlist->fileList[i];
         QString fileValue = getFileValue(wdif->getSourceName());
         getKeyWords(fileValue,wvtwordlist);
         wvtwordlist->closeDocument(wdif);
     }
     return wvtwordlist;
 }
 //call the ICTCLAS to get keyWords in the file and put them in the wordlist
 void WVTool::getKeyWords(const QString content,WVTWordList* wvtwordlist)
 {
     QLibrary lib("ICTCLAS30.dll");
     if(lib.load())
     {
         //QString content1("my name si chen yajun");
         std::string aaa = content.toStdString();
         const char* sInput =aaa.c_str();
         //const char *sInput = "my name si chen yajun";
//         if(!ICTCLAS_Init())
//         {
//                 printf("ICTCLAS INIT FAILED!\n");
//                 return ;
//         }
//
//         int nCount = ICTCLAS_GetParagraphProcessAWordCount(sInput);
//
//         //split words and extract the keyword
//         result_t *result =(result_t*)malloc(sizeof(result_t)*nCount);
//         ICTCLAS_ParagraphProcessAW(nCount,result);//Get the results to the client's memory kept
//
//         //extract the keyword,should call after the function ICTCLAS_ParagraphProcessAW
//         result_t *resultKey = (result_t*)malloc(sizeof(result_t)*nCount);
//         int nCountKey;
//         ICTCLAS_KeyWord(resultKey, nCountKey);
//
//         for (int i=0; i<nCountKey; i++)
//         {
//             bool isnum = false;
//                 char buf[100];
//                 memset(buf, 0, 100);
//                 int index = resultKey[i].start;
//                 memcpy(buf,(void *)(sInput+index), resultKey[i].length);
//                QString stringbuf(buf);
//                //std::cout << stringbuf.toStdString() << endl;
////                for(int i = 0;i < stringbuf.length();i++)
////                {
////                    if(stringbuf.at(i).isNumber())
////                    {
////                        isnum = true;
////                        break;
////                    }
////                }
//               if(!stopWords.contains(stringbuf) && !isnum)
//               {
//                    wvtwordlist->addWordOccurance(stringbuf);
//               }
//         }
//
//         free(resultKey);
//         free(result);

         //初始化分词组件
                 if(!ICTCLAS_Init())
                 {
                         printf("ICTCLAS INIT FAILED!\n");
                         return ;
                 }

                 ICTCLAS_SetPOSmap(ICT_POS_MAP_SECOND);

                 const char *sResult = ICTCLAS_ParagraphProcess(sInput, 1);
                 QString str(sResult);
                  QStringList list1 = str.split(" ");
                  foreach (QString str, list1)
                  {
                       string str1 = str.toStdString();
                       string str2 = str1.substr(0,str1.find('/'));
                       //cout << str2 << endl;
                       QString stringbuf(QObject::tr(str2.c_str()));
                       QString buf1 = stringbuf.trimmed().toLower();
                       if(!STOPWORDS->contains(buf1) && (buf1 != ""))
                           {
                              wvtwordlist->addWordOccurance(buf1);
                           }
                  }


         //free the resources of the word compontent
         ICTCLAS_Exit();
      }
 }

 //get content in the file
QString WVTool::getFileValue(const QString& filePath)
{
    QFile file(filePath);

            if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
            {
                return "";
            }
            QString content("");
            while(!file.atEnd())
            {
                QByteArray bytea = file.readLine();
                QString str(bytea);
                content += str;
            }
            //std::cout << content.toStdString() << endl;
            file.flush();
            file.close();

            return content;
}
//
void WVTool::createVectors(WVTFileInputList* wvtinputlist,WVTWordList* wvtwordlist,QFile& path)
{
    wvtwordlist->setAppendWords(false);
    wvtwordlist->setUpdateOnlyCurrent(true);
    for(int i = 0;i < wvtinputlist->fileList.size();i++)
    {
        WVTDocumentInfo* wdif = wvtinputlist->fileList[i];
        QString fileValue = getFileValue(wdif->getSourceName());
        getKeyWords(fileValue,wvtwordlist);
        TFIDF tfidf;
        WVTWordVector* wvtvector = tfidf.createVector(wvtwordlist->getFrequenciesForCurrentDocument(),wvtwordlist->getTermCountForCurrentDocument(),wvtwordlist,wdif);
        write(wvtvector,wvtwordlist,path);
        wvtwordlist->closeDocument(wdif);
    }
}
//
QString WVTool::createVector(QString value, WVTWordList *wvtwordlist)
{
    wvtwordlist->setAppendWords(false);
    wvtwordlist->setUpdateOnlyCurrent(true);
WVTDocumentInfo* wdif = new WVTDocumentInfo("",0);
    getKeyWords(value,wvtwordlist);
    TFIDF tfidf;
    WVTWordVector* wvtvector = tfidf.createVector(wvtwordlist->getFrequenciesForCurrentDocument()
                                                  ,wvtwordlist->getTermCountForCurrentDocument(),wvtwordlist,wdif);
    QString result = this->getVector(wvtvector,wvtwordlist);
     wvtwordlist->closeDocument(wdif);
     delete wdif;wdif = 0;
     return result;
}
//
void WVTool::write(WVTWordVector* wvtwordvector,WVTWordList* wvtwordlist,QFile& path)
{
    int j = wvtwordvector->getDocumentInfo()->getClassValue();


    QTextStream out(&path);
    out << j;
    int length = wvtwordlist->wordList.size();
    double* ad = wvtwordvector->getValues();
     for(int k = 0; k < length; k++)
     {
         if(ad[k] > 0.0)
             out << " " << k <<":" << ad[k];
     }
     out << "\n";
    delete wvtwordvector;
    wvtwordvector = 0;
}
//
QString WVTool::getVector(WVTWordVector* wvtwordvector,WVTWordList* wvtwordlist)
{
    int j = wvtwordvector->getDocumentInfo()->getClassValue();
    QString result;
    result += QString().setNum(j);
    int length = wvtwordlist->wordList.size();
    double* ad = wvtwordvector->getValues();
     for(int k = 0; k < length; k++)
     {
         if(ad[k] > 0.0)
             result += QString(" ") + QString().setNum(k)+":"+QString().setNum(ad[k]);
     }
     delete wvtwordvector;
     wvtwordvector = 0;

     return result;
}
