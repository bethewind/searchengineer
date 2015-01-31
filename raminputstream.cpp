#include "raminputstream.h"
#include "ramfile.h"
#include <iostream>
#include "util.h"
using namespace std;
RAMInputStream::RAMInputStream()
{
}
RAMInputStream::~RAMInputStream()
{
    DELETEARRAY(currentBuffer);
}
 RAMInputStream::RAMInputStream(const RAMInputStream& clone)
 {
    this->bufferLength = clone.bufferLength;
    this->bufferPosition = clone.bufferPosition;
    this->bufferStart = clone.bufferStart;
    this->currentBufferIndex = clone.currentBufferIndex;
    this->file = clone.file;
    this->length = clone.length;
    if(clone.currentBuffer != 0)
    {
        unsigned char* currentBuffer1 = clone.currentBuffer;
        this->currentBuffer = new unsigned char[BUFFER_SIZE];
        for(int i = 0;i < BUFFER_SIZE;i++)
        {
            this->currentBuffer[i] = currentBuffer1[i];
        }
    }
    else
    {
        this->currentBuffer = 0;
    }
 }
//
 RAMInputStream::RAMInputStream(RAMFile* f):bufferPosition(0)
         ,bufferLength(0),bufferStart(0)
{
   file = f;
   length = file->length;

   // make sure that we switch to the
   // first needed buffer lazily
   currentBufferIndex = -1;
   currentBuffer = 0;
 }
//
unsigned char RAMInputStream::readByte()
{
    if (bufferPosition >= bufferLength) {
      currentBufferIndex++;
      switchCurrentBuffer();
    }
    return currentBuffer[bufferPosition++];
 }
//
void RAMInputStream::switchCurrentBuffer()
{
    if (currentBufferIndex >= file->buffers.size())
    {
      // end of file reached, no more buffers left
      cout << "RAMInputStream::switchCurrentBuffer()Read past EOF\n";
    } else
    {
      currentBuffer = (unsigned char*) file->buffers.value(currentBufferIndex);
      bufferPosition = 0;
      bufferStart = BUFFER_SIZE * currentBufferIndex;
      bufferLength = (int) (length - bufferStart);
      if (bufferLength > BUFFER_SIZE) {
        bufferLength = BUFFER_SIZE;
      }
    }
 }
//
void RAMInputStream::seek(__int64 pos)
{

    __int64 bufferStart = currentBufferIndex * BUFFER_SIZE;
    if (pos < bufferStart || pos >= bufferStart + BUFFER_SIZE) {
      currentBufferIndex = (int) (pos / BUFFER_SIZE);
      switchCurrentBuffer();
    }
    bufferPosition = (int) (pos % BUFFER_SIZE);

}
//
IndexInput* RAMInputStream::clone()
{
    RAMInputStream* ret = new RAMInputStream(*this);
       return ret;
}
//
void RAMInputStream::readBytes(unsigned char *b, int offset, int len)
{
    for(int i = offset;i < len;i++)
    {
        b[i] = this->readByte();
    }
}
