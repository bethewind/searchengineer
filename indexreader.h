#ifndef INDEXREADER_H
#define INDEXREADER_H
#include <QString>
#include "termenum.h"
class SegmentInfos;
class Directory;
class FieldInfos;
class Document;
class TermPositions;
class IndexReader
{
public:
    IndexReader();
   virtual ~IndexReader();
    void init(Directory* directory, SegmentInfos* segmentInfos, bool closeDirectory
              , bool directoryOwner);
    Directory* getDirectory() {return directory;}
    void close();
   virtual FieldInfos* getField()=0;
   virtual bool hasNorms(QString field) = 0;
   virtual int maxDoc()=0;
   virtual Document* document(int n)=0;
   //Returns the number of documents in this index.
   virtual int numDocs()=0;
   //Returns an enumeration of all the terms in the index. The enumeration is
   //ordered by Term.compareTo(). Each term is greater than all that precede it
   //in the enumeration. Note that after calling terms(), TermEnum.next() must
   //be called on the resulting enumeration before
   //calling other methods such as TermEnum.term().
   virtual TermEnum* terms() = 0;
   virtual TermPositions* termPositions()=0;
   virtual void getNorms(QString field, unsigned char* bytes, int offset)=0;
private:
    Directory* directory;
    bool directoryOwner;
    bool closeDirectory;
    SegmentInfos* segmentInfos;
    bool closed;
    bool hasChanges;
protected:
    virtual void doClose() = 0;
};

#endif // INDEXREADER_H
