#ifndef SEGMENTTERMDOCS_H
#define SEGMENTTERMDOCS_H
#include "indexinput.h"
#include "termenum.h"
#include "terminfo.h"
#include "term.h"
#include "termdocs.h"
class SegmentReader;
class SegmentTermDocs : public virtual TermDocs
{
public:
    SegmentTermDocs();
    SegmentTermDocs(SegmentReader* parent);
    void seek(TermEnum* termEnum);
    virtual void seek(TermInfo* ti, Term* term);
    bool next();
    int getDoc(){return doc;}
    int getFreq(){return freq;}
    void close();

    int doc;
    int freq;
protected:
      SegmentReader* parent;
      IndexInput* freqStream;
      int count;
      int df;
private:
      int skipInterval;
      int maxSkipLevels;
      __int64 freqBasePointer;
      __int64 proxBasePointer;

      __int64 skipPointer;
      bool haveSkipped;
};

#endif // SEGMENTTERMDOCS_H
