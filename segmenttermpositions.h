#ifndef SEGMENTTERMPOSITIONS_H
#define SEGMENTTERMPOSITIONS_H
#include "segmenttermdocs.h"
#include "termpositions.h"

class SegmentReader;
class SegmentTermPositions : public SegmentTermDocs,public TermPositions
{
public:
    SegmentTermPositions();
    SegmentTermPositions(SegmentReader* p);
    void seek(TermInfo* ti, Term* term);
    bool next();
    int nextPosition();
    void close();
    //void seek(TermEnum* termEnum){SegmentTermDocs::seek(termEnum);}
private :
     __int64 lazySkipPointer;
     int lazySkipProxCount;
     IndexInput* proxStream;
     int proxCount;

     void lazySkip();
     int readDeltaPosition();
     int position;
};

#endif // SEGMENTTERMPOSITIONS_H
