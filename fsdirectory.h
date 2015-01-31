#ifndef FSDIRECTORY_H
#define FSDIRECTORY_H
#include "directory.h"
#include <QString>
#include <QFile>
#include <QMap>
#include <QMutex>
#include "bufferedindexoutput.h"

class IndexOutput;
class FSDirectory : public Directory
{
   class FSIndexOutput : public BufferedIndexOutput
    {
    public:
        QFile* file;
        FSIndexOutput();
         FSIndexOutput(QFile* path);
         ~FSIndexOutput();
  void seek(__int64 pos);


         void close();
    private:
         bool isOpen;
    protected:
         void flushBuffer(unsigned char* b, int len);
         void flushBuffer(unsigned char *b,int offset, int len);
    };
public:
    FSDirectory();
    ~FSDirectory();
    static FSDirectory* getDirectory(const QString& path);
    IndexOutput* createOutput(const QString& name);
     void deleteFile(QString name){}
     void close();
     IndexInput* openInput(QString name){return 0;}
     bool fileExists(QString name){return false;}
     QString getDir(){return this->directory;}


private:
    static QMap<QString,FSDirectory*> * DIRECTORIES;
    QMutex mutex;
    QString directory;
    int refCount;
};

#endif // FSDIRECTORY_H
