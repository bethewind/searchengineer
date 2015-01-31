#ifndef RAMINPUTSTREAM_H
#define RAMINPUTSTREAM_H
#include <_mingw.h>
#include "ramoutputstream.h"
#include "indexinput.h"
class RAMFile;
class RAMInputStream : public IndexInput
{
public:
    RAMInputStream();
    ~RAMInputStream();
    RAMInputStream(RAMFile* f);
    RAMInputStream(const RAMInputStream& clone);
    unsigned char readByte();
    void readBytes(unsigned char* b, int offset, int len);
    static const int BUFFER_SIZE = RAMOutputStream::BUFFER_SIZE;
    void close(){}
    __int64 getLength(){return length;}
    RAMFile* getFile(){return file;}
    int getCuBuIndex(){return this->currentBufferIndex;}
    unsigned char* getCuBuffer(){return this->currentBuffer;}
    int getBuPosition(){return this->bufferPosition;}
    __int64 getBuStart(){return this->bufferStart;}
    int getBuLength(){return this->bufferLength;}
    void seek(__int64 pos);
    IndexInput* clone();
private:
    RAMFile* file;
    __int64 length;
    int currentBufferIndex;
    unsigned char* currentBuffer; 
    int bufferPosition;
    __int64 bufferStart;
    int bufferLength;

    void switchCurrentBuffer();
};

#endif // RAMINPUTSTREAM_H
