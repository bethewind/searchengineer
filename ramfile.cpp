#include "ramfile.h"
#include "util1.h"
#include "ramdirectory.h"
RAMFile::RAMFile():directory(0),sizeInBytes(0),length(0)
{
}
RAMFile::~RAMFile()
{
    for(int i = 0;i < buffers.size();i++)
    {
        if(buffers[i]!=0)
        {
            delete[] buffers[i];
            buffers[i] = 0;
        }
    }
}
RAMFile::RAMFile(RAMDirectory* directory):sizeInBytes(0)
{
   this->directory = directory;
   length = 0;
   lastModified = Util::currentTimeMillis();
 }
//
unsigned char* RAMFile::addBuffer(int size)
{
    unsigned char* buffer = new unsigned char[size];
    if (directory!=0)
      {             // Ensure addition of buffer and adjustment to directory size are atomic wrt directory
        buffers.append(buffer);
        directory->sizeInBytes += size;
        sizeInBytes += size;
      }
    else
      buffers.append(buffer);
    return buffer;
  }
