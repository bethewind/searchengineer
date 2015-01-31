#include "defaultskiplistwriter.h"
#include "util.h"

DefaultSkipListWriter::DefaultSkipListWriter(int skipInterval, int numberOfSkipLevels, int docCount
                                             , IndexOutput* freqOutput, IndexOutput* proxOutput)
    :MultiLevelSkipListWriter(skipInterval,numberOfSkipLevels, docCount),curDoc(0)
    ,curFreqPointer(0),curProxPointer(0)
{
       this->freqOutput = freqOutput;
       this->proxOutput = proxOutput;

       lastSkipDoc = new int[numberOfSkipLevels];
       lastSkipFreqPointer = new __int64[numberOfSkipLevels];
       lastSkipProxPointer = new __int64[numberOfSkipLevels];

       this->maxSkipLevel = numberOfSkipLevels;
}
//
DefaultSkipListWriter::~DefaultSkipListWriter()
{
   DELETEARRAY(lastSkipDoc);
   DELETEARRAY(lastSkipFreqPointer);
   DELETEARRAY(lastSkipProxPointer);
}
//
void DefaultSkipListWriter::resetSkip()
{
    MultiLevelSkipListWriter::resetSkip();
    for(int i = 0;i < maxSkipLevel;i++)
    {
        lastSkipDoc[i] = 0;
        lastSkipFreqPointer[i] = freqOutput->getFilePointer();
        lastSkipProxPointer[i] = proxOutput->getFilePointer();

    }
}
//
void DefaultSkipListWriter::setSkipData(int doc)
{
    this->curDoc = doc;
    this->curFreqPointer = freqOutput->getFilePointer();
    this->curProxPointer = proxOutput->getFilePointer();
}
//
void DefaultSkipListWriter::writeSkipData(int level, IndexOutput* skipBuffer)
{
          // current field does not store payloads
          skipBuffer->writeVInt(curDoc - lastSkipDoc[level]);

        skipBuffer->writeVInt((int) (curFreqPointer - lastSkipFreqPointer[level]));
        skipBuffer->writeVInt((int) (curProxPointer - lastSkipProxPointer[level]));

        lastSkipDoc[level] = curDoc;
        //System.out.println("write doc at level " + level + ": " + curDoc);

        lastSkipFreqPointer[level] = curFreqPointer;
        lastSkipProxPointer[level] = curProxPointer;
}
