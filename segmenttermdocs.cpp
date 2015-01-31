#include "segmenttermdocs.h"
#include "segmentreader.h"
#include "terminfosreader.h"
#include "segmenttermenum.h"
#include "fieldinfos.h"
SegmentTermDocs::SegmentTermDocs()
{
}
SegmentTermDocs::SegmentTermDocs(SegmentReader* parent)
    :doc(0),freq(0),count(0),df(0),freqBasePointer(0)
    ,proxBasePointer(0),skipPointer(0),haveSkipped(false)
{
    this->parent = parent;
    this->freqStream = parent->freqStream->clone();
    this->skipInterval = parent->tis->getSkipInterval();
    this->maxSkipLevels = parent->tis->getMaxSkipLevels();
  }
//Sets this to the data for the current term in a TermEnum. This may be
//optimized in some implementations.
void SegmentTermDocs::seek(TermEnum *termEnum)
{
    TermInfo* ti;
    Term* term;
    // use comparison of fieldinfos to verify that termEnum belongs to the same segment as this SegmentTermDocs
    if (SegmentTermEnum* segmentTermEnum1 = dynamic_cast<SegmentTermEnum*>(termEnum))
       {
          if(segmentTermEnum1->fieldInfos == parent->fieldInfos)
          {
            // optimized case
            term = segmentTermEnum1->term();
            ti = segmentTermEnum1->getTermNewInfo();
          }
       }
    else
    {                                         // punt case
         //term = termEnum->term();
         //ti = parent.tis.get(term);
    }

    seek(ti, term);
}
//
void SegmentTermDocs::seek(TermInfo* ti, Term* term)
{
    count = 0;
    //FieldInfo* fi = parent->fieldInfos->fieldInfo(term->field);
    if (ti == 0) {
      df = 0;
    } else {
      df = ti->docFreq;
      doc = 0;
      freqBasePointer = ti->freqPointer;
      proxBasePointer = ti->proxPointer;
      skipPointer = freqBasePointer + ti->skipOffset;
      freqStream->seek(freqBasePointer);
      haveSkipped = false;
    }
  }
//
bool SegmentTermDocs::next()
{
   // while (true) {
      if (count == df)
        return false;

      int docCode = freqStream->readVInt();
      doc += ((unsigned int)docCode) >> 1;       // shift off low bit
      if ((docCode & 1) != 0)       // if low bit is set
        freq = 1;         // freq is one
      else
        freq = freqStream->readVInt();     // else read freq

      count++;

//      if (deletedDocs == null || !deletedDocs.get(doc))
//        break;
//      skippingDoc();
   // }
    return true;
}
//
void SegmentTermDocs::close()
{

    freqStream->close();
//    if (skipListReader != 0)
//      skipListReader->close();

}

