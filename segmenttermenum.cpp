#include "segmenttermenum.h"
#include "indexinput.h"
#include "terminfoswriter.h"
#include <iostream>
#include "util.h"
SegmentTermEnum::SegmentTermEnum()
{
}
SegmentTermEnum::~SegmentTermEnum()
{

    DELETE(termBuffer);
     DELETE(prevBuffer);
      DELETE(scratch);
       DELETE(termInfo);

       //if ( isClone )
            DELETE(input);
}
SegmentTermEnum::SegmentTermEnum(IndexInput *i, FieldInfos *fis, bool isi)
    :position(-1),indexPointer(0),termBuffer(new TermBuffer())
    ,prevBuffer(new TermBuffer()),scratch(new TermBuffer()),termInfo(new TermInfo())
    ,isIndex(false),isClone(false)
{

    input = i;
    fieldInfos = fis;
    isIndex = isi;
    maxSkipLevels = 1; // use single-level skip lists for formats > -3

    int firstInt = input->readInt();

      // we have a format version number
      format = firstInt;

      // check that it is a format we can understand
      if (format < TermInfosWriter::FORMAT)
          std::cout << "Unknown format version\n";

      size = input->readLong();                    // read the size(Term count)
        indexInterval = input->readInt();
        skipInterval = input->readInt();
          maxSkipLevels = input->readInt();
}
//clone this instance
SegmentTermEnum::SegmentTermEnum(const SegmentTermEnum& clone):
                fieldInfos(clone.fieldInfos)
        {
                input = clone.input->clone();
                //Copy the postion from the clone
                position     = clone.position;
                termBuffer = clone.termBuffer==0 ? 0 : new TermBuffer(*(clone.termBuffer));
                 this->prevBuffer = clone.prevBuffer==0 ? 0 : new TermBuffer(*(clone.prevBuffer));
                  this->scratch = clone.scratch==0 ? 0 : new TermBuffer(*(clone.scratch));
                isIndex      = clone.isIndex;
                termInfo     = clone.termInfo == 0 ? 0 : new TermInfo(clone.termInfo);
                indexPointer = clone.indexPointer;
                size         = clone.size;

      format       = clone.format;
      indexInterval= clone.indexInterval;
      skipInterval = clone.skipInterval;
      this->maxSkipLevels = clone.maxSkipLevels;
      formatM1SkipInterval = clone.formatM1SkipInterval;
                //Set isClone to true as this instance is a clone of another instance
                isClone = true;
        }
//
void SegmentTermEnum::close()
{
    //input->close();
    DOCLOSE(input);
}
//Returns a clone of this instance
SegmentTermEnum* SegmentTermEnum::clone()
{
    return new SegmentTermEnum(*this);
 }
//Returns the current Term in the enumeration. Initially invalid, valid after next() called
//for the first time.
Term* SegmentTermEnum::term()
{
    return this->termBuffer->toTerm();
}
//
bool SegmentTermEnum::next()
{
    if(position++ >= size - 1)
    {
        this->termBuffer->reset();
        this->prevBuffer->reset();
        return false;
    }
    this->prevBuffer->set(termBuffer);
    this->termBuffer->read(input,fieldInfos);

    termInfo->docFreq = input->readVInt();	  // read doc freq
    termInfo->freqPointer += input->readVLong();	  // read freq pointer
    termInfo->proxPointer += input->readVLong();	  // read prox pointer

    if (termInfo->docFreq >= skipInterval)
            termInfo->skipOffset = input->readVInt();

    if (isIndex)
          indexPointer += input->readVLong();	  // read index pointer

    return true;
}
//
TermInfo* SegmentTermEnum::getTermNewInfo()
{
    return new TermInfo(this->termInfo);
}
