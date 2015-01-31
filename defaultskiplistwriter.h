#ifndef DEFAULTSKIPLISTWRITER_H
#define DEFAULTSKIPLISTWRITER_H
#include "multilevelskiplistwriter.h"
#include "indexoutput.h"

class DefaultSkipListWriter : public MultiLevelSkipListWriter
{
public:
    DefaultSkipListWriter(int skipInterval, int numberOfSkipLevels, int docCount
                          , IndexOutput* freqOutput, IndexOutput* proxOutput);
    ~DefaultSkipListWriter();
    void setSkipData(int doc);
    void resetSkip();
    void writeSkipData(int level, IndexOutput* skipBuffer);
    int maxSkipLevel;
private:
    int* lastSkipDoc;//array
    __int64* lastSkipFreqPointer;//array
    __int64* lastSkipProxPointer;//array

    IndexOutput* freqOutput;
    IndexOutput* proxOutput;

    int curDoc;
    __int64 curFreqPointer;
    __int64 curProxPointer;

};

#endif // DEFAULTSKIPLISTWRITER_H
