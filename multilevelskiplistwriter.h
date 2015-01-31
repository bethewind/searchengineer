#ifndef MULTILEVELSKIPLISTWRITER_H
#define MULTILEVELSKIPLISTWRITER_H
#include "indexoutput.h"
class RAMOutputStream;
class MultiLevelSkipListWriter
{
public:
    MultiLevelSkipListWriter();
    ~MultiLevelSkipListWriter();
    MultiLevelSkipListWriter(int skipInterval, int maxSkipLevels, int df);
    void bufferSkip(int df);
    __int64 writeSkip(IndexOutput* output);

private:
    // number of levels in this skip list
    int numberOfSkipLevels;

      // the skip interval in the list with level = 0
    int skipInterval;

      // for every skip level a different buffer is used
    RAMOutputStream** skipBuffer;//array
    int skipBufferLength;
protected :
    void resetSkip();
    void init();
    virtual void writeSkipData(int level, IndexOutput* skipBuffer)=0;
};

#endif // MULTILEVELSKIPLISTWRITER_H
