#include "ramoutputstream.h"
#include "ramfile.h"
#include "util1.h"
RAMOutputStream::RAMOutputStream():file(new RAMFile()),bufferPosition(0)
        ,bufferStart(0),bufferLength(0)
{
    currentBufferIndex = -1;
    currentBuffer = 0;
    deletefile = true;
}
RAMOutputStream::~RAMOutputStream()
{
    if(this->deletefile)
    {
        if(file != 0)
        {
            delete file;file = 0;
        }
    }
}
RAMOutputStream::RAMOutputStream(RAMFile* f)
    :bufferPosition(0),bufferStart(0),bufferLength(0)
{
  file = f;

  // make sure that we switch to the
  // first needed buffer lazily
  currentBufferIndex = -1;
  currentBuffer = 0;
  deletefile = false;
}
//
void RAMOutputStream::writeByte(unsigned char b)
{

    if (bufferPosition == bufferLength) {
      currentBufferIndex++;
      switchCurrentBuffer();
    }
    currentBuffer[bufferPosition++] = b;

}
//
void RAMOutputStream::writeBytes(unsigned char *b, int offset, int len)
{

    for (int i = offset; i < len; i++)
          writeByte(b[i]);

}
void RAMOutputStream::close()
{
    flush();
}
//
void RAMOutputStream::flush()
{
    file->setLastModified(Util::currentTimeMillis());
    setFileLength();
}
//switch to the currentBuffer
void RAMOutputStream::switchCurrentBuffer()
{

    if (currentBufferIndex == file->buffers.size())
    {
      currentBuffer = file->addBuffer(BUFFER_SIZE);
    } else
    {
      currentBuffer = (unsigned char*) file->buffers.value(currentBufferIndex);
    }
    bufferPosition = 0;
    bufferStart = BUFFER_SIZE * currentBufferIndex;
    bufferLength = BUFFER_SIZE;

}
//
void RAMOutputStream::setFileLength()
{
    __int64 pointer = bufferStart + bufferPosition;
    if (pointer > file->length)
    {
      file->setLength(pointer);
    }
 }
//
void RAMOutputStream::seek(__int64 pos)
{
    setFileLength();
    if (pos < bufferStart || pos >= bufferStart + bufferLength)
    {
          currentBufferIndex = (int) (pos / BUFFER_SIZE);
          switchCurrentBuffer();
    }

        bufferPosition = (int) (pos % BUFFER_SIZE);
}
//Resets this to an empty buffer.
void RAMOutputStream::reset()
{
    seek(0);
    file->setLength(0);
}
//
void RAMOutputStream::writeTo(IndexOutput* out)
{
    flush();
    __int64 end = file->length;
    __int64 pos = 0;
    int buffer = 0;
    while (pos < end) {
      int length = BUFFER_SIZE;
      __int64 nextPos = pos + length;
      if (nextPos > end) {                        // at the last buffer
        length = (int)(end - pos);
      }
      out->writeBytes(file->buffers.value(buffer++), length);
      pos = nextPos;
    }
}
