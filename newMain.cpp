//#include "page.h"
//#include <QString>
//#include <QUrl>
//#include "htmlparser.h"
//#include <QtNetwork>
//#include <iostream>
//#include <fstream>
//#include <QTextCodec>
//#include <QtCore/QCoreApplication>
//#include <iostream>
//#include <fstream>
//#include <string>
//#include <QStringList>
//#include <QFile>
//#include <QDir>
//#include <QTextStream>
//#include "tag.h"
//#include <stdlib.h>
//#include <io.h>
//#include <QDateTime>
//#include <QTime>
//#include <QMap>
//#include <QMapIterator>
//#include <QTextCodec>
//#include <QObject>
//using namespace std;
//void print(string content);
//int main(int argc, char *argv[])
//{
//    QCoreApplication a1(argc, argv);
//    QTextCodec::setCodecForTr(QTextCodec::codecForLocale());
//        QFile file("example1.html");
//
//        QString href;//ÍøÒ³url
//        int pageCNum; //Àà±ð±àºÅ
//        string html; //ÍøÒ³ÄÚÈÝ
//        int pageNum=0; //ÍøÒ³±àºÅ
//        int flag=0;  //0ÎªÑµÁ·£¬1Îª²âÊÔ
//
//            unsigned char* uc;
//           if(file.open(QIODevice::ReadOnly | QIODevice::Text))
//             {
//                while(!file.atEnd())
//               {
//                       if(pageNum == 1222)
//                   {
//                       int a = 1;
//                    }
//                       uc = new unsigned char[5];
//                       _read(file.handle(),uc,5);
//                        //pageNum = QString((const char*)uc).trimmed().toInt();
//                       pageNum ++;
//                       delete[] uc;uc = 0;
//   //                    if(pageNum == 1309)
//   //                    {
//   //                        int a = 1;
//   //                    }
//                       uc = new unsigned char[6];
//                       _read(file.handle(),uc,6);
//                       pageCNum = QString((const char*)uc).trimmed().toInt();
//                        delete[] uc;uc = 0;
//                       uc = new unsigned char[255];
//                       _read(file.handle(),uc,255);
//                       href = QString((const char*)uc);
//                        delete[] uc;uc = 0;
//                       uc = new unsigned char[1];
//                       _read(file.handle(),uc,1);
//                        //flag = QString((const char*)uc).trimmed().toInt();
//                        delete[] uc;uc = 0;
//
//                       uc = new unsigned char[7];
//                       _read(file.handle(),uc,7);
//                       int le = QString((const char*)uc).trimmed().toInt();
//                        delete[] uc;uc = 0;
//                       uc = new unsigned char[le];
//                       _read(file.handle(),uc,le);
//                       //out << QObject::tr((const char*)uc);
//                       //QString ccc(QObject::tr((const char*)uc));
//                       html = QString((const char*)uc).toStdString();
////                         if(pageNum == 1809)
////                       {
////                             print(html);
////                         }
//                       //print(html);
//                       //cout << html;
//                       delete[] uc;uc = 0;
//
//                       if(html.find("<html") == std::string::npos)
//                       {
//                            if(html.find("<HTML") == std::string::npos)
//                                continue;
//                       }
//                       if(html.find("<body") == std::string::npos)
//                       {
//                            if(html.find("<BODY") == std::string::npos)
//                                continue;
//                       }
//                       int begin = html.find('<');
//                       int end = html.rfind('>');
//                       std::string st1 = html.substr(begin,end-begin+1);
//                        QUrl* url = new QUrl(href,QUrl::TolerantMode);
//                        HtmlParser htmlParser;
//                        string content;
////                        if(pageNum == 1809)
////                    {
////                        int a = 1;
////                     }
//                       if(le > 100)
//                        {
//                               Page page(url,st1,htmlParser);
//                               content = page.getImportantText();
//
//                              if(content.length() < 100)
//                                  continue;
//                         }
//                       else
//                       {
//                           continue;
//                       }
//                       QString data = QString("data")+QString().setNum(flag)+QString("/");
//                       QString path = data+QString().setNum(pageCNum);
//                       QDir dir1(path);
//                       QString path1 = dir1.absolutePath();
//                          if(!dir1.exists())
//                          {
//                               std::cout << "can not find the directory"+ path.toStdString()+",create it!\n";
//                              if(!dir1.mkpath(path1))
//                                  std::cout << "can not make directory"+ path.toStdString();
//                          }
//                           QString filePath = dir1.absolutePath()+QString("/")+QString().setNum(pageNum);
//                        QFile file1(filePath);
//                        if(!file1.open(QIODevice::WriteOnly | QIODevice::Text))
//                        {
//                            cout << "can not open the file " << pageNum << endl;
//                        }
//                        QTextStream out(&file1);
//                        out << QObject::tr(content.c_str());
//                        file1.flush();
//                        file1.close();
//                         file.flush();
//   //                      if(pageNum == 6530)
//   //                          break;
//                }
//             }
//
//
//
//                   file.close();
//
//           system("pause");
//           return a1.exec();
//           return 0;
//}
//void print(string content)
//{
//    QFile file1("1.txt");
//    if(!file1.open(QIODevice::WriteOnly | QIODevice::Text))
//    {
//        //cout << "can not open the file " << pageNum << endl;
//    }
//    QTextStream out(&file1);
//    out << QObject::tr(content.c_str());
//    file1.flush();
//    file1.close();
//}
