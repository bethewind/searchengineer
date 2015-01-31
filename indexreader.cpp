#include "indexreader.h"
#include "directory.h"

IndexReader::IndexReader()
{
}
IndexReader::~IndexReader()
{
}
//
void IndexReader::init(Directory* directory, SegmentInfos* segmentInfos, bool closeDirectory
          , bool directoryOwner)
{
  this->directory = directory;
  this->segmentInfos = segmentInfos;
  this->directoryOwner = directoryOwner;
  this->closeDirectory = closeDirectory;
}
//
void IndexReader::close()
{
    if(!closed)
    {
        hasChanges = false;
        doClose();
        if (directoryOwner)
             closed = true;
        if(closeDirectory)
              directory->close();
    }
}
