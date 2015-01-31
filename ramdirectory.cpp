#include "ramdirectory.h"
#include "ramfile.h"
#include "ramoutputstream.h"
#include <QMapIterator>
#include <iostream>
#include "raminputstream.h"
using namespace std;
RAMDirectory::RAMDirectory() : sizeInBytes(0)
{
}
RAMDirectory::~RAMDirectory()
{
    QMapIterator<QString,RAMFile*> i(fileMap);
    while (i.hasNext()) {
        i.next();
        RAMFile* rf = i.value();
       delete rf;rf = 0;
    }
    fileMap.clear();
}
//
IndexOutput* RAMDirectory::createOutput(const QString& name)
{
     RAMFile* file = new RAMFile(this);
     if(fileMap.contains(name))
     {
         RAMFile* existing = fileMap.value(name);
         sizeInBytes -= existing->sizeInBytes;
         delete existing;
         existing = 0;
     }
     fileMap.insert(name,file);
     return new RAMOutputStream(file);
}
//
IndexInput* RAMDirectory::openInput(QString name)
{
    RAMFile* file = 0;
      file = fileMap.value(name);
    if (file == 0)
      cout << "RAMDirectory::openInput;can not find the file!\n";
    return new RAMInputStream(file);
  }
//Returns true iff the named file exists in this directory.
bool RAMDirectory::fileExists(QString name)
{
    //RAMFile* file = 0;
    return fileMap.contains(name);
}
//Removes an existing file in the directory.
void RAMDirectory::deleteFile(QString name)
{
    if(fileMap.contains(name))
    {
        RAMFile* file = fileMap.value(name);
        delete file;file = 0;
        fileMap.remove(name);
        sizeInBytes -= file->sizeInBytes;     // updates to RAMFile.sizeInBytes synchronized on directory
    }
    else
    {
        std::cout << "RAMDirectory::deleteFile(QString name);can not find the file\n";
    }

}
