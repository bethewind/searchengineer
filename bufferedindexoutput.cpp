#include "bufferedindexoutput.h"

BufferedIndexOutput::BufferedIndexOutput() : bufferStart(0),bufferPosition(0)
,buffer(new unsigned char[BUFFER_SIZE])
{
}
BufferedIndexOutput::~BufferedIndexOutput()
{}
//Writes a single byte.
void BufferedIndexOutput::writeByte(unsigned char b)
{
    if (bufferPosition >= BUFFER_SIZE)
      flush();
    buffer[bufferPosition++] = b;
 }
//Forces any buffered output to be written.
void BufferedIndexOutput::flush()
{
    flushBuffer(buffer, bufferPosition);
        bufferStart += bufferPosition;
        bufferPosition = 0;
}
//
void BufferedIndexOutput::close()
{
    flush();
}
//
void BufferedIndexOutput::seek(__int64 pos)
{
    flush();
    bufferStart = pos;
}
//
void BufferedIndexOutput::writeBytes(unsigned char *b, int offset, int length)
{

    for (int i = offset; i < length; i++)
          writeByte(b[i]);

}
