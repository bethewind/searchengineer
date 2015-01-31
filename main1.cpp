//#include <QtCore/QCoreApplication>
//#include "page.h"
//#include <QString>
//#include <QUrl>
//#include <QtNetwork>
//#include <QTextCodec>
//#include <iostream>
//#include <string>
//#include <QFile>
//#include <stdlib.h>
//#include <QObject>
//#include <QVector>
//#include "htmlparser.h"
//using namespace std;
//int main(int argc, char *argv[])
//{
//    QCoreApplication a(argc, argv);
//    QVector<QString> fetchQueue;
//    fetchQueue.append("http://localhost/");
//
//        //while(fetchQueue.size() !=0)
//        {
//            QString u = fetchQueue.at(0);
//            QUrl url(u);
//            fetchQueue.remove(0);
//            QString host = url.host();
//            QString ho = "http://"+host+"";
//            QString path = u.replace(ho,"");
//            const int Timeout = 5 * 1000;
//            QTcpSocket socket;
//             socket.connectToHost(host,80);
//
//            if (!socket.waitForConnected(Timeout))
//             {
//                 std::cout << "can't connect to the host"
//                         << host.toStdString() << "\n";
//                 //continue;
//             }
//                QString link = "GET /";
//                 link += path;
//                 link += " HTTP/1.1\nHost:";
//                 link += host;
//                 link += "\nConnection: Close\n\n";
//                 std::string s = link.toStdString();
//                 const char* link1 = s.c_str();
//                 socket.write(link1);
//                 if (!socket.waitForReadyRead(5000))
//                 {
//                     std::cout << "can't read from the host"
//                              << u.toStdString() << "\n";
//                     //continue;
//                 }
//
//                 int numRead = 0, numReadTotal = 0;
//                 //QTextCodec *tc = QTextCodec::codecForName("utf-8");
//                  //QTextCodec *tc1 = QTextCodec::codecForName("GBK");
//                 char buffer[51];
//                  QString string1("");
//                  forever
//                  {
//                      numRead  = socket.read(buffer, 50);
//
//                      if(numRead != 0)
//                        {
//
//               //          QByteArray  st222 = tc1->fromUnicode(tc->toUnicode(buffer));
//               //          QString st111(st222);
//               //          cout << st111.toStdString();
//                           cout << buffer;
//                           QString string2(buffer);
//                           string1 += string2;
//                         }
//
//                          numReadTotal += numRead;
//                       if (numRead == 0 && !socket.waitForReadyRead())
//                           break;
//                    }
//                  std::string st = string1.toStdString();
//                 int begin = st.find('<');
//                 int end = st.rfind('>');
//                std::string html = st.substr(begin,end-begin+1);
//                HtmlParser parser;
//               string mainText;
//               string title1;
//
//                   Page page(&url,html,parser);
//                  mainText = page.getImportantText();
//                  title1 = page.getTitle();
//                  for(int i = 0;i < page.links.size();i++)
//                  {
//                      fetchQueue.append(page.links[i]->getUrl().toString());
//                  }
//
//
//        }
//        system("pause");
//    return a.exec();
//}
