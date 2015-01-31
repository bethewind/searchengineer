#include "page.h"
#include <QString>
#include <QUrl>
#include "htmlparser.h"
#include <QtNetwork>
#include <iostream>
#include <fstream>
#include <QTextCodec>
#include <string>
#include "tag.h"
#include "svmpredict.h"
#include "indexwriter.h"
#include "chineseanalyzer.h"
#include "document.h"
#include "field.h"
#include "priorityqueue.h"
#include "linkqueue.h"
#include "qdebug.h"
using namespace std;
QByteArray getPageCode(const string& pageContent);
int main(int argc, char *argv[])
{
    QCoreApplication a1(argc, argv);
    QTextCodec::setCodecForTr(QTextCodec::codecForLocale());
    QVector<QString> crawledQueue;
    LinkQueue fetchQueue(10000);
    Link* linktxt;
    int totalLinkNum=0;
    QFile file("data/url.txt");
      if(file.open(QIODevice::ReadOnly | QIODevice::Text))
       {
          while(!file.atEnd())
          {
              QByteArray bytea = file.readLine();
              QString str(QObject::tr(QString(bytea).toStdString().c_str()));
              //std::cout << str.toStdString();
              linktxt = new Link(str,0,++totalLinkNum);
              fetchQueue.append(linktxt);
          }
       }
            file.flush();
            file.close();
     QMap<QString,QByteArray> pageCode;

     SvmPredict predict;
     IndexWriter iWriter("myIndex",new ChineseAnalyzer(),true);

   while(true)
   {
       if(fetchQueue.getSize() == 0)
           break;
       Link* cuLink = fetchQueue.pop();
       QString u,u1,u2;
       u = u1 = u2 = cuLink->getText();
       crawledQueue.append(u);
       QString u11 = u1.replace("http://","");
       QUrl url1(u);
       QString host = url1.host();
       if( u11== host)
           u.append("/");
       QString ho = "http://"+host+"/";
       QString path = u2.replace(ho,"");
       const int Timeout = 5 * 1000;
       QTcpSocket socket;
        socket.connectToHost(host,80);

              if (!socket.waitForConnected(Timeout))
                {
                  std::cout << "can't connect to the host"
                                           << host.toStdString() << "\n";

                         continue;
                     }
              QString link = "GET /";
               link += path;
               link += " HTTP/1.1\nHost:";
               link += host;
               link += "\nConnection: Close\n\n";
               std::string s = link.toStdString();
               const char* link1 = s.c_str();
               socket.write(link1);
            //socket.write("GET /system/2010/06/03/004740524.shtml HTTP/1.1\nHost:news.enorth.com.cn\nConnection: Close\n\n");


                         if (!socket.waitForReadyRead(Timeout)) {
                             std::cout << "can't read from the host"
                                                          << u.toStdString() << "\n";

                              continue;
                         }


                         int numRead = 0, numReadTotal = 0;
                         QTextCodec *tc;// = QTextCodec::codecForName("utf-8");
                          QTextCodec *tc1 = QTextCodec::codecForLocale();//QTextCodec::codecForName("GBK");

                             char buffer[51];
                            QString string1("");
                             forever {
                                 numRead  = socket.read(buffer, 50);

                                if(numRead != 0)
                                 {

                                    //QByteArray  st222 = tc1->fromUnicode(tc->toUnicode(buffer));
                                    //QString st111(st222);
   //                                 cout << st111.toStdString();
                                    //cout << buffer;
                                    QString string2(buffer);
                                    string1 += string2;
                                }

                                 numReadTotal += numRead;
                                 if (numRead == 0 && !socket.waitForReadyRead())
                                     break;
                             }

                                     std::string stt = string1.toStdString();
                                     if(stt.find('<') == std::string::npos)
                                         continue;

                                     string st = stt;

                                     QByteArray code;
                                     if(pageCode.contains(host))
                                     {
                                         code = pageCode.value(host);
                                     }
                                     else
                                     {
                                         code = getPageCode(stt);
                                         pageCode.insert(host,code);
                                     }

                                     if(code != "gb2312" && code != "gbk")
                                     {
                                         tc = QTextCodec::codecForName(code);
                                         QByteArray  st222 = tc1->fromUnicode(tc->toUnicode(stt.c_str()));
                                          QString st111(st222);
                                           st = st111.toStdString();
                                     }

                                     if(st.find("<html") == std::string::npos)
                                     {
                                          if(st.find("<HTML") == std::string::npos)
                                              continue;
                                     }
                                     if(st.find("<body") == std::string::npos)
                                     {
                                          if(st.find("<BODY") == std::string::npos)
                                              continue;
                                     }
                                     int begin = st.find('<');
                                     int end = st.rfind('>');
                                     std::string st1 = st.substr(begin,end-begin+1);


              QString href(u);
           string html = st1;

           QUrl* url = new QUrl(href,QUrl::TolerantMode);
           QString aaaaa = url->host();
           HtmlParser htmlParser;


          Page page(url,html,htmlParser);
          for(int i = 0;i < page.links.size();i++)
          {
              QString ll = page.links[i]->getUrl().toString();
              if(crawledQueue.contains(ll))
              {
                  continue;
              }
              Link* newLink;
              string ll1 = ll.toStdString();
              if(ll1.rfind('.') != std::string::npos)
              {
                 int begin = ll1.rfind('.');
                  std::string st1 = ll1.substr(begin,ll1.length()-begin);
                  if(!ll.startsWith("javascript") && !HtmlParser::isNoLinks(st1) && !ll.endsWith("#"))
                  {
                      newLink = new Link(ll,cuLink->getDepth()+1,++totalLinkNum);
                      fetchQueue.append(newLink);
                  }

              }
              else
              {
                  newLink = new Link(ll,cuLink->getDepth()+1,++totalLinkNum);
                  fetchQueue.append(newLink);
              }
          }

          string  mainText = page.getImportantText();
          if(mainText.length() < 200)
              continue;
        string title1 = page.getTitle();
       cout << "抓取网页:" << title1 << endl;
       cout << "---------------网页正文-----------------\n" << endl;

       //cout << mainText << endl;
       qDebug() << QObject::tr(mainText.c_str()) << "\n";
      int category1 = predict.predict(QString(mainText.c_str()));
       cout << "---------------网页类别-----------------\n" << endl;
       cout << "网页类别:"<< category1 << "---------1.娱乐与休闲2.计算机与因特网3.医疗与健康" << endl; //

       Document* document = new Document();
       Field* category = new Field(QObject::tr("category"),QString().setNum(category1),Field::StoreYES,Field::IndexUN_TOKENIZED);
       Field* title = new Field(QObject::tr("title"),QObject::tr(title1.c_str()),Field::StoreYES,Field::IndexUN_TOKENIZED);
       Field* pageUrl = new Field(QObject::tr("pageUrl"),u,Field::StoreYES,Field::IndexUN_TOKENIZED);
       Field* content = new Field(QObject::tr("content"),mainText.c_str(),Field::StoreYES,Field::IndexTOKENIZED);
            document->add(category);
            document->add(title);
            document->add(pageUrl);
            document->add(content);

            iWriter.addDocument(document);

             cout << "为网页创建索引完毕！" << endl;
           //break;


   }

        system("pause");
         return a1.exec();
        return 0;
}
QByteArray getPageCode(const string& pageContent)
{

   string sub = pageContent.substr(0,pageContent.find('<'));
   if(sub.find("charset=")==std::string::npos)
       return "gbk";
   string code = sub.substr(sub.find("charset=")+8,sub.length());
   string code1 = code.substr(0,code.find('\n'));
   QString tem = QString(code1.c_str()).trimmed();
   QByteArray aaa = tem.toAscii();
   return aaa;
}
