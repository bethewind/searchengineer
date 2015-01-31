#include "segmenttermpositions.h"
#include "segmentreader.h"
SegmentTermPositions::SegmentTermPositions()
{
}
SegmentTermPositions::SegmentTermPositions(SegmentReader *p)
    :SegmentTermDocs(p),lazySkipPointer(0),lazySkipProxCount(0)
{
    proxStream = 0;
}
//
void SegmentTermPositions::seek(TermInfo* ti, Term* term)
{
    SegmentTermDocs::seek(ti, term);
    if (ti != 0)
      lazySkipPointer = ti->proxPointer;

    lazySkipProxCount = 0;
    proxCount = 0;
}
//
bool SegmentTermPositions::next()
{
    this->lazySkipProxCount += this->proxCount;

    if(SegmentTermDocs::next())
    {
        this->proxCount = freq;
        position = 0;
        return true;
    }
    return false;
}
//
int SegmentTermPositions::nextPosition()
{
    lazySkip();
    proxCount--;
    return position += readDeltaPosition();
}
//
void SegmentTermPositions::lazySkip()
{
    if(this->proxStream == 0)
        this->proxStream = parent->proxStream->clone();
}
//
int SegmentTermPositions::readDeltaPosition()
{
    int delta = proxStream->readVInt();
    return delta;
}
//
void SegmentTermPositions::close()
{
    SegmentTermDocs::close();
    if (proxStream != 0) proxStream->close();
}
