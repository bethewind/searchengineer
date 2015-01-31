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
//
//int main(int argc, char *argv[])
//{
//    QDir dir("D:\\eclipseapp\\html-purifier\\debug\\data0\\14");
//           dir.setFilter(QDir::Files | QDir::Hidden | QDir::NoSymLinks);
//           dir.setSorting(QDir::Size | QDir::Reversed);
//           QFileInfoList list = dir.entryInfoList();
//         for (int j = 0; j < list.size(); ++j)
//           {
//                  QFileInfo fileInfo = list.at(j);
//                  int size = fileInfo.size();
//                  if(size <= 100)
//                  {
//                       QString abpath = fileInfo.absoluteFilePath();
//                       QFile file(abpath);
//                       file.remove();
//                  }
//           }
//         system("pause");
//         return 0;
//}
