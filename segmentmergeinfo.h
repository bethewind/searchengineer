#ifndef SEGMENTMERGEINFO_H
#define SEGMENTMERGEINFO_H
#include "termenum.h"
#include "term.h"
#include "indexreader.h"
#include "termpositions.h"

class SegmentMergeInfo
{
public:
    SegmentMergeInfo();
    ~SegmentMergeInfo();
    SegmentMergeInfo(int b,TermEnum* te,IndexReader* r);
    bool next();
    void close();
    TermPositions* getPositions();

      Term* term;
      int base;
      TermEnum* termEnum;
      IndexReader* reader;
private:
      TermPositions* postings;  // use getPositions()
      int* docMap;  //(array) use getDocMap()
};

#endif // SEGMENTMERGEINFO_H
