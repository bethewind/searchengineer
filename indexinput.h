#ifndef INDEXINPUT_H
#define INDEXINPUT_H
#include <QString>
class IndexInput
{
public:
    IndexInput();
    int readVInt();
    virtual unsigned char readByte() = 0;
    int readInt();
    __int64 readLong();
    __int64 readVLong();
    QString readString();
    void readChars(QString& buffer, int start, int length);
    virtual void close()=0;
    virtual __int64 getLength()=0;
    virtual void seek(__int64 pos)=0;
    virtual IndexInput* clone() =0;
    virtual void readBytes(unsigned char* b, int offset, int len)=0;
private:
    QString chars;
};

#endif // INDEXINPUT_H
