#include "terminfo.h"

TermInfo::TermInfo():docFreq(0),freqPointer(0),proxPointer(0),skipOffset(0)
{
}
TermInfo::TermInfo(TermInfo* ti)
{
   docFreq = ti->docFreq;
   freqPointer = ti->freqPointer;
   proxPointer = ti->proxPointer;
   skipOffset = ti->skipOffset;
 }
//
void TermInfo::set(int docFreq,__int64 freqPointer, __int64 proxPointer, int skipOffset)
{
    this->docFreq = docFreq;
    this->freqPointer = freqPointer;
    this->proxPointer = proxPointer;
    this->skipOffset = skipOffset;
}
//
void TermInfo::set(TermInfo* ti)
{
    docFreq = ti->docFreq;
    freqPointer = ti->freqPointer;
    proxPointer = ti->proxPointer;
    skipOffset = ti->skipOffset;
  }

