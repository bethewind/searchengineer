#ifndef TERMINFO_H
#define TERMINFO_H
#include "_mingw.h"
class TermInfo
{
public:
    TermInfo();
    TermInfo(TermInfo* ti);
    void set(int docFreq,__int64 freqPointer, __int64 proxPointer, int skipOffset);
    void set(TermInfo* ti);

    int docFreq;
      __int64 freqPointer;
      __int64 proxPointer;
      int skipOffset;
};

#endif // TERMINFO_H
