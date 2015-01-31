#include "terminfoswriter.h"
#include "directory.h"
#include <iostream>
#include "util1.h"
#include "util.h"
#include "fieldinfos.h"
using namespace std;
TermInfosWriter::TermInfosWriter()
{
}
TermInfosWriter::~TermInfosWriter()
{
    DELETE(lastTerm);
}
TermInfosWriter::TermInfosWriter(Directory* directory, QString segment, FieldInfos* fis,
                                 int interval):indexInterval(128)
,skipInterval(3),maxSkipLevels(10),lastTerm(new Term("",""))
,lastTi(new TermInfo()),size(0),lastIndexPointer(0)
,isIndex(false),other(0)
{
    initialize(directory, segment, fis, interval, false);
        other = new TermInfosWriter(directory, segment, fis, interval, true);
        other->other = this;
}
//
TermInfosWriter::TermInfosWriter(Directory* directory, QString segment, FieldInfos* fis,
                                 int interval,bool isIndex):indexInterval(128)
,skipInterval(3),maxSkipLevels(10),lastTerm(new Term("",""))
,lastTi(new TermInfo()),size(0),lastIndexPointer(0)
,isIndex(false),other(0)
{
    initialize(directory, segment, fis, interval, isIndex);
}
//
void TermInfosWriter::initialize(Directory* directory, QString segment, FieldInfos* fis,
                          int interval, bool isi)
{
    indexInterval = interval;
        fieldInfos = fis;
        isIndex = isi;
        output = directory->createOutput(segment + (isIndex ? QString(".tii") : QString(".tis")));
        output->writeInt(FORMAT);                      // write format
        output->writeLong(0);                          // leave space for size
        output->writeInt(indexInterval);             // write indexInterval
        output->writeInt(skipInterval);              // write skipInterval
        output->writeInt(maxSkipLevels);              // write maxSkipLevels
}
//
//
void TermInfosWriter::add(Term* term,TermInfo* ti)
{

    if (!isIndex && term->compareTo(lastTerm) <= 0)
     cout << "term out of order\n";
    if (ti->freqPointer < lastTi->freqPointer)
         cout << "freqPointer out of order\n";
    if (ti->proxPointer < lastTi->proxPointer)
        cout << "proxPointer out of order\n";

    if (!isIndex && size % indexInterval == 0)
      other->add(lastTerm, lastTi);                      // add an index term

    writeTerm(term);                                    // write term
    output->writeVInt(ti->docFreq);                       // write doc freq
    output->writeVLong(ti->freqPointer - lastTi->freqPointer); // write pointers
    output->writeVLong(ti->proxPointer - lastTi->proxPointer);

    if (ti->docFreq >= skipInterval) {
      output->writeVInt(ti->skipOffset);
    }

    if (isIndex) {
      output->writeVLong(other->output->getFilePointer() - lastIndexPointer);
      lastIndexPointer = other->output->getFilePointer(); // write pointer
    }

    lastTi->set(ti);
    size++;

}
//
void TermInfosWriter::writeTerm(Term* term)
   {
    int start = Util::stringDifference(lastTerm->text, term->text);
    int length = term->text.length() - start;

    output->writeVInt(start);                   // write shared prefix length
    output->writeVInt(length);                  // write delta length
    output->writeChars(term->text, start, length);  // write delta chars

    output->writeVInt(fieldInfos->fieldNumber(term->field)); // write field num

    DELETE(lastTerm);
    lastTerm = new Term(term->field,term->text);
  }
//
void TermInfosWriter::close()
{
    output->seek(4);
    output->writeLong(size);
    output->close();
    if(!isIndex)
        other->close();
}
