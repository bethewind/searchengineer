#ifndef RAMDIRECTORY_H
#define RAMDIRECTORY_H
#include "directory.h"
#include <QMap>

class RAMFile;
class RAMDirectory : public Directory
{
public:
    RAMDirectory();
    ~RAMDirectory();
    IndexOutput* createOutput(const QString& name);
    IndexInput* openInput(QString name);
    bool fileExists(QString name);
    void deleteFile(QString name);
    void close(){}
    QString getDir(){return "";}

    QMap<QString,RAMFile*> fileMap;
    __int64 sizeInBytes;
};

#endif // RAMDIRECTORY_H
