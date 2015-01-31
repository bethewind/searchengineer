#ifndef INDEXOUTPUT_H
#define INDEXOUTPUT_H
#include <QString>
class IndexOutput
{
public:
    IndexOutput();
    virtual ~IndexOutput();
    void writeInt(int i);
    void writeVInt(int vi);
    void writeVLong(__int64 vi);
    void writeLong(__int64 i);
    virtual void writeByte(unsigned char b)=0;
    void writeString(QString s);
    void writeChars(QString s, int start, int length);
    void writeBytes(unsigned char* b, int length);
    virtual void writeBytes(unsigned char* b, int offset, int length)=0;
    virtual void close() = 0;
    virtual __int64 getFilePointer()=0;
    virtual  void seek(__int64 pos)=0;
};

#endif // INDEXOUTPUT_H
