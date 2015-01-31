#include "segmentinfos.h"
#include "indexfilenames.h"
#include "indexoutput.h"
#include "directory.h"
#include "segmentinfo.h"
#include "util1.h"
#include "util.h"
SegmentInfos::SegmentInfos():counter(0),generation(-1),lastGeneration(-1)
        ,version(Util::currentTimeMillis())
{
}
//
void SegmentInfos::write(Directory *directory)
{
     QString segmentFileName = getNextSegmentFileName();
     if (generation == -1)
         {
           generation = 1;
         }
     else
         {
           generation++;
         }
      IndexOutput* output = directory->createOutput(segmentFileName);
      bool success = false;

      try
      {
          output->writeInt(CURRENT_FORMAT); // write FORMAT
               output->writeLong(++version); // every write changes
                                            // the index
               output->writeInt(counter); // write counter
               output->writeInt(this->size()); // write infos
               for (int i = 0; i < size(); i++)
                 info(i)->write(output);
      }
      catch(...)
      {
          output->close();
          //success = true;
          delete output;
          output = 0;
          throw;
      }
      output->close();
      success = true;
      delete output;
      output = 0;

      try
      {
          output = directory->createOutput(IndexFileNames::SEGMENTS_GEN);
          output->writeInt(FORMAT_LOCKLESS);
          output->writeLong(generation);
          output->writeLong(generation);
      }
      catch(...){}
      output->close();
      delete output;
      output = 0;
      lastGeneration = generation;
}
//
QString SegmentInfos::getNextSegmentFileName()
{
    __int64 nextGeneration;
    if(this->generation == -1)
    {
        nextGeneration = 1;
    }
    else
    {
        nextGeneration = this->generation + 1;
    }
    QString tem = IndexFileNames::SEGMENTS;
    tem += "_";
    QString tem1;
    tem1.setNum(nextGeneration,36);
    tem += tem1;
    return tem;
}
//
SegmentInfo* SegmentInfos::info(int i)
{
    return this->at(i);
}
//
void SegmentInfos::removeAllElements()
{
    for(int i = 0;i < this->size();i++)
    {
        SegmentInfo* tem = this->at(i);
        if(tem != 0)
        {
            delete tem;tem = 0;
            this->replace(i,0);
        }
    }
    this->clear();
}
