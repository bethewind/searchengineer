#ifndef BUFFEREDINDEXOUTPUT_H
#define BUFFEREDINDEXOUTPUT_H
#include "indexoutput.h"
class BufferedIndexOutput : public IndexOutput
{
public:
    BufferedIndexOutput();
    void writeByte(unsigned char b);
    void writeBytes(unsigned char* b, int offset, int length);
    void close();
    void flush();
    virtual ~BufferedIndexOutput();
    __int64 getFilePointer() {return bufferStart + bufferPosition;}
    void seek(__int64 pos);

    static const int BUFFER_SIZE = 16384;
private:
    __int64 bufferStart;           // position in file of buffer
    int bufferPosition;         // position in buffer
     unsigned char* buffer;
protected:
    virtual void flushBuffer(unsigned char* b, int len) = 0;
};

#endif // BUFFEREDINDEXOUTPUT_H
