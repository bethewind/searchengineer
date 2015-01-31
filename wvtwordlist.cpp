#include "wvtwordlist.h"
#include "wvtword.h"
#include "wvtdocumentinfo.h"
#include <QList>
#include <QTextStream>
#include <QFile>
#include <sstream>
#include <iostream>
#include <QTextCodec>
#include <string>
#include <iostream>
#include <QStringList>
using namespace std;
WVTWordList::WVTWordList(int i)
    :numClasses(0),appendWords(true),updateOnlyCurrent(true),numDocuments(0)
    ,ZNumDocuments(0),numLocalTerms(0)
{
    numClasses = i;
}
//
WVTWordList::WVTWordList(const QString& filePath)
    :numClasses(0),appendWords(true),updateOnlyCurrent(true),numDocuments(0)
    ,ZNumDocuments(0),numLocalTerms(0)
{
    QFile file(filePath);

            if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
            {
                std::cout << "can not open the file!WVTWordList::WVTWordList\n";

            }

            QByteArray bytea = file.readLine();
           QString str(QObject::tr(QString(bytea).toStdString().c_str()));
           QStringList list = str.split(" ");
           QListIterator<QString> i(list);
            if(i.next() == "@number_of_documents")
           {
               this->numDocuments = i.next().toInt();
           }
            QByteArray bytea1 = file.readLine();
           QString str1(QObject::tr(QString(bytea1).toStdString().c_str()));
           QStringList list1 = str1.split(" ");
           QListIterator<QString> i1(list1);
            if(i1.next() == "@number_of_classes")
           {
               this->numClasses = i1.next().toInt();
           }

            QString content("");
            while(!file.atEnd())
            {
                QByteArray bytea2 = file.readLine();
                 QString str2(QObject::tr(QString(bytea2).toStdString().c_str()));
                 QStringList list2 = str2.split(",");
                 QListIterator<QString> i2(list2);
                 QString word = i2.next();
                 WVTWord* wvtword = new WVTWord(word, numClasses);
                 wordList.append(wvtword);
                 wordMap.insert(word,wvtword);
                 if(i2.hasNext())
                 {
                     wvtword->setDocumentFrequency(i2.next().toInt());
                     int m = 0;
                     while(m < numClasses)
                     {
                         if(i2.hasNext())
                         {
                             wvtword->setClassFrequency(m,i2.next().toInt());
                         }
                         m++;
                     }
                 }
            }
            //std::cout << content.toStdString() << endl;
            file.flush();
            file.close();
}
WVTWordList::~WVTWordList()
{
    QList<WVTWord*> wordList = wordMap.values();
    for(int i = 0;i < wordList.size();i++)
    {
        delete wordList[i];
        wordList[i] = 0;
    }
}
void WVTWordList::addWordOccurance(const QString& s)
{
    WVTWord* wvtword = 0;
   if(wordMap.contains(s))
   {
        wvtword = wordMap.value(s);

   }
   else
   {
       if(appendWords)
       {
            wvtword = new WVTWord(s, numClasses);
            wordList.append(wvtword);
            wordMap.insert(s,wvtword);
       }
   }
   if(wvtword != 0)
   {
       wvtword->addOccurance(); //the number of the term int the current doc
   }
    numLocalTerms++; //the total number of terms contain in the current doc.
}
//
void WVTWordList::closeDocument(WVTDocumentInfo* wvtdocumentinfo)
{
    for(int i = 0; i < wordList.size(); i++)
        wordList[i]->closeDocument(wvtdocumentinfo->getClassValue(),updateOnlyCurrent);

            if(!updateOnlyCurrent)
            {
                numDocuments++;
                if(wvtdocumentinfo->getClassValue() > 0)
                {
                    ZNumDocuments += 1;
                }
            }
            numLocalTerms = 0;
}
//prune keyWords byFrequency
void WVTWordList::pruneByFrequency(int i, int j)
{
    wordList.clear();
   QList<WVTWord*> list = wordMap.values();
    for(int k = 0;k < list.size();k++)
    {
        WVTWord* wvtword = list.at(k);
        if(wvtword->getDocumentFrequency() > i && wvtword->getDocumentFrequency() < j)
        {
            wordList.append(wvtword);
        }

    }
}
void WVTWordList::store(const QString& path)
{
    QFile file(path);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return;

    QTextStream out(&file);
    out << "number_of_documents " << numDocuments << "\n";
    out << "number_of_classes " << numClasses << "\n";
//    string change;
//    std::stringstream stream;
//    string st = "number_of_documents ";
//    stream << numDocuments;
//    stream >> change;
//    st += change;
//    st += "\n";
//    st += "number_of_classes ";
//    stream.clear();
//    stream << numClasses;
//    stream >> change;
//    st += change;
//    st += "\n";
//    file.write(st.c_str());
     //std::cout << wordList.size() << endl;
    for(int i = 0; i < wordList.size(); i++)
       {
          WVTWord* wvtword = wordList.at(i);
          //std::cout << wvtword->getWord().toStdString() << endl;
          out << wvtword->getWord() << "," << wvtword->getDocumentFrequency();
//          string st1("");
//          st1 += wvtword->getWord().toStdString();
//          st1 += ",";
//          stream.clear();
//         stream << wvtword->getDocumentFrequency();
//         stream >> change;
//         st1 += change;
          for(int j = 0; j < numClasses; j++)
          {
//              st1 += ",";
//              stream.clear();
//              stream << wvtword->getClassFrequency(j);
//              stream >> change;
//              st1 += change;
              out << "," << wvtword->getClassFrequency(j);
          }
//                  st1 += "\n";
//                  file.write(st1.c_str());
                    out << "\n";
        }
    file.flush();
    file.close();
}
//get the nums in this document of erery word in the Wordlist
int* WVTWordList::getFrequenciesForCurrentDocument()
    {
        int* ai = new int[wordList.size()];
        for(int i = 0; i < wordList.size(); i++)
            ai[i] = wordList.at(i)->getLocalFrequency();

        return ai;
    }
//get the nums of document contains the word
int* WVTWordList::getDocumentFrequencies()
    {
    int* ai = new int[wordList.size()];
    for(int i = 0; i < wordList.size(); i++)
        ai[i] = wordList.at(i)->getDocumentFrequency();

    return ai;
    }
