#include "segmentmergeinfo.h"
#include "util.h"
SegmentMergeInfo::SegmentMergeInfo()
{
}
SegmentMergeInfo::~SegmentMergeInfo()
{
    DELETEARRAY(this->docMap);
}
SegmentMergeInfo::SegmentMergeInfo(int b, TermEnum *te, IndexReader *r)
    :postings(0)
{
    base = b;
    reader = r;
    termEnum = te;
    term = te->term();
}
//
bool SegmentMergeInfo::next()
{

    if (termEnum->next())
    {
      term = termEnum->term();
      return true;
    } else
    {
      term = 0;
      return false;
    }

}
//
void SegmentMergeInfo::close()
{
    this->termEnum->close();
    if(this->postings != 0)
        postings->close();
}
//
TermPositions* SegmentMergeInfo::getPositions()
{
    if(this->postings == 0)
        this->postings = reader->termPositions();

    return this->postings;
}
