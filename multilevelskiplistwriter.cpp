#include "multilevelskiplistwriter.h"
#include <cmath>
#include "util.h"
#include "ramoutputstream.h"
#include "ramfile.h"
#include <iostream>
using namespace std;
MultiLevelSkipListWriter::MultiLevelSkipListWriter():numberOfSkipLevels(0)
{
}
MultiLevelSkipListWriter::~MultiLevelSkipListWriter()
{
    if(skipBuffer != 0)
    {
        for(int i = 0;i < this->skipBufferLength;i++)
        {
            DELETE(skipBuffer[i]);
        }
    }
    DELETEARRAY(skipBuffer);
}
//
MultiLevelSkipListWriter::MultiLevelSkipListWriter(int skipInterval, int maxSkipLevels
                                                   , int df):skipBuffer(0),skipBufferLength(0)
{
    this->skipInterval = skipInterval;

    // calculate the maximum number of skip levels for this document frequency
    numberOfSkipLevels = df == 0 ? 0 : (int)floor(log(df) / log(skipInterval));

    // make sure it does not exceed maxSkipLevels
    if (numberOfSkipLevels > maxSkipLevels) {
      numberOfSkipLevels = maxSkipLevels;
    }
}
//
void MultiLevelSkipListWriter::resetSkip()
{
    if(this->skipBuffer == 0)
    {
        init();
    }
    else
    {

        for (int i = 0; i < skipBufferLength; i++)
        {
          skipBuffer[i]->reset();
        }

    }
}
//
void MultiLevelSkipListWriter::init()
 {
    skipBuffer = new RAMOutputStream*[numberOfSkipLevels];
    this->skipBufferLength = numberOfSkipLevels;
    for (int i = 0; i < numberOfSkipLevels; i++) {
      skipBuffer[i] = new RAMOutputStream();
    }
  }
/*
   * Writes the current skip data to the buffers. The current document frequency determines
   * the max level is skip data is to be written to.
   *
   * @param df the current document frequency
   * @throws IOException
   */
void MultiLevelSkipListWriter::bufferSkip(int df)
  {
    int numLevels;

    // determine max level
    for (numLevels = 0; (df % skipInterval) == 0 && numLevels < numberOfSkipLevels; df /= skipInterval) {
      numLevels++;
    }

    __int64 childPointer = 0;

    for (int level = 0; level < numLevels; level++) {
      writeSkipData(level, skipBuffer[level]);

      __int64 newChildPointer = skipBuffer[level]->getFilePointer();

      if (level != 0) {
        // store child pointers for all levels except the lowest
        skipBuffer[level]->writeVLong(childPointer);
      }

      //remember the childPointer for the next level
      childPointer = newChildPointer;
    }
  }

//
__int64 MultiLevelSkipListWriter::writeSkip(IndexOutput* output)
{
    __int64 skipPointer = output->getFilePointer();
    if (skipBuffer == 0 || this->skipBufferLength == 0) return skipPointer;

    for (int level = numberOfSkipLevels - 1; level > 0; level--)
    {
      __int64 length = skipBuffer[level]->getFilePointer();
      if (length > 0) {
        output->writeVLong(length);
        skipBuffer[level]->writeTo(output);
      }
    }
    skipBuffer[0]->writeTo(output);

    return skipPointer;
}
