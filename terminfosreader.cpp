#include "terminfosreader.h"
#include "directory.h"
#include "segmenttermenum.h"
#include "util.h"
TermInfosReader::TermInfosReader()
{

}
TermInfosReader::~TermInfosReader()
{
    DELETEARRAY(indexTerms);DELETEARRAY(indexInfos);DELETEARRAY(indexPointers);
    DELETE(origEnum); DELETE(indexEnum);
}
TermInfosReader::TermInfosReader(Directory *dir, QString seg
                                 , FieldInfos *fis):indexTerms(0)
{
    directory = dir;
    segment = seg;
    fieldInfos = fis;
    origEnum = new SegmentTermEnum(directory->openInput(segment + ".tis"),fieldInfos, false);
    size = origEnum->size;
    indexEnum = new SegmentTermEnum(directory->openInput(segment + ".tii"),fieldInfos, true);
}
//
void TermInfosReader::close()
{

    DOCLOSE(origEnum); DOCLOSE(indexEnum);
}
//Returns an enumeration of all the Terms and TermInfos in the set.
 SegmentTermEnum* TermInfosReader::terms()
 {
     SegmentTermEnum* cln = origEnum->clone();
     return cln;
 }
 //
 int TermInfosReader::getSkipInterval()
 {
     return origEnum->skipInterval;
 }
 //
 int TermInfosReader::getMaxSkipLevels()
 {
     return origEnum->maxSkipLevels;
 }
