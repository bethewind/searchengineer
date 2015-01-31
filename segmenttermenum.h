#ifndef SEGMENTTERMENUM_H
#define SEGMENTTERMENUM_H
#include "termbuffer.h"
#include "terminfo.h"
#include "termenum.h"

class FieldInfos;
class IndexInput;
class SegmentTermEnum : public TermEnum
{
public:
    SegmentTermEnum();
    SegmentTermEnum(IndexInput* i, FieldInfos* fis, bool isi);
    SegmentTermEnum(const SegmentTermEnum& clone);
    ~SegmentTermEnum();
    void close();
    SegmentTermEnum* clone();
    TermBuffer* getTermBuffer(){return termBuffer;}
    TermBuffer* getPrevBuffer(){return prevBuffer;}
    TermBuffer* getScratch(){return scratch;}
    TermInfo* getTermInfo(){return termInfo;}
    IndexInput* getInput(){return input;}
    TermInfo* getTermNewInfo();
    Term* term();
    //Increments the enumeration to the next element.
    //True if one exists.
    bool next();

    FieldInfos* fieldInfos;
    __int64 size;
    __int64 position;
    __int64 indexPointer;
      int indexInterval;
      int skipInterval;
      int maxSkipLevels;
private:
    TermBuffer* termBuffer;
    TermBuffer* prevBuffer;
    TermBuffer* scratch;                     // used for scanning
    TermInfo* termInfo;
    int format;
    bool isIndex;
    int formatM1SkipInterval;
    IndexInput* input;
    bool isClone;
};

#endif // SEGMENTTERMENUM_H
