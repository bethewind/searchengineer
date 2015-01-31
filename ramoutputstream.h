#ifndef RAMOUTPUTSTREAM_H
#define RAMOUTPUTSTREAM_H
#include "indexoutput.h"

class RAMFile;
class RAMOutputStream : public IndexOutput
{
public:
    RAMOutputStream();
    ~RAMOutputStream();
    RAMOutputStream(RAMFile* f);
    void writeByte(unsigned char b);
    void close();
    void flush();
    void switchCurrentBuffer();
    void seek(__int64 pos);
    __int64 getFilePointer() {return currentBufferIndex < 0 ? 0 : bufferStart + bufferPosition;}
    void reset();
    void writeTo(IndexOutput* out);
    void writeBytes(unsigned char* b, int offset, int len);

    static const int BUFFER_SIZE = 1024;
private:
    RAMFile* file;
    int currentBufferIndex;
    unsigned char* currentBuffer;
    int bufferPosition;
    __int64 bufferStart;
    int bufferLength;
    bool deletefile;

    void setFileLength();
};

#endif // RAMOUTPUTSTREAM_H
