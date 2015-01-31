#include "fsdirectory.h"
#include <QDir>
#include <iostream>
#include <io.h>

QMap<QString,FSDirectory*> * FSDirectory::DIRECTORIES = new QMap<QString,FSDirectory*>();
FSDirectory::FSDirectory():refCount(0)
{
}
FSDirectory::~FSDirectory()
{

}
void FSDirectory::close()
{

    if (--refCount <= 0)
    {
        DIRECTORIES->remove(directory);
    }

}
//(static)Returns the directory instance for the named location.
FSDirectory* FSDirectory::getDirectory(const QString& path)
{
    QDir dir1(path);
    if(!dir1.exists())
    {
         std::cout << "can not find the directory"+ path.toStdString();
        if(!dir1.mkpath(path))
            std::cout << "can not make directory"+ path.toStdString();
    }
    QString abPath = dir1.absolutePath();
    FSDirectory* dir = new FSDirectory();
    if(!DIRECTORIES->contains(abPath))
    {

        dir->directory = abPath+QString("/") ;
        DIRECTORIES->insert(abPath,dir);
    }
    dir->refCount++;
    return dir;

}
//Creates a new, empty file in the directory with the given name. Returns a stream writing this file.
IndexOutput* FSDirectory::createOutput(const QString& name)
{
    QString str = this->directory +name;
    QFile* file = new QFile(str);
    if(file->exists() && !file->remove())
    {
        std::cout << "cannot overwrite the file";
    }
    IndexOutput* temindex = new FSDirectory::FSIndexOutput(file);
    return temindex;
}


//
void FSDirectory::FSIndexOutput::close()
{
    // only close the file if it has not been closed yet
          if (isOpen) {
              BufferedIndexOutput::close();
            file->close();
            isOpen = false;
          }
}

//fsindexoutput
FSDirectory::FSIndexOutput::FSIndexOutput()
{
}
//
FSDirectory::FSIndexOutput::FSIndexOutput(QFile* path)
{
    file = path;
    if(!file->open(QIODevice::WriteOnly | QIODevice::Text))
    {
        std::cout << "cannot open the file";
    }

    isOpen = true;
}
FSDirectory::FSIndexOutput::~FSIndexOutput()
{
    delete file;
    file = 0;
}
//
void FSDirectory::FSIndexOutput::flushBuffer(unsigned char *b, int len)
{
    flushBuffer(b,0,len);
}
//
void FSDirectory::FSIndexOutput::flushBuffer(unsigned char *b,int offset, int len)
{
     if (file->isOpen())
    {
          _write(file->handle(),b+offset,len);
    }
}
//
//
void FSDirectory::FSIndexOutput::seek(__int64 pos)
{

    BufferedIndexOutput::seek(pos);
   file->seek(pos);

}


