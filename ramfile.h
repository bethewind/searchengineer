#ifndef RAMFILE_H
#define RAMFILE_H
#include <QVector>
class RAMDirectory;
class RAMFile
{
public:
    RAMFile();
    ~RAMFile();
    RAMFile(RAMDirectory* directory);
    unsigned char* addBuffer(int size);
    void setLastModified(__int64 lastModified) {this->lastModified = lastModified;}
    void setLength(__int64 length) {this->length = length;}
    //
    RAMDirectory* directory;
    __int64 length;
    QVector<unsigned char*> buffers;
    __int64 lastModified;
    __int64 sizeInBytes;
};

#endif // RAMFILE_H
