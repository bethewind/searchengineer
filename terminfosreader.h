#ifndef TERMINFOSREADER_H
#define TERMINFOSREADER_H
#include <QString>
#include "term.h"
#include "terminfo.h"
class Directory;
class FieldInfos;
class SegmentTermEnum;
class TermInfosReader
{
public:
    TermInfosReader();
    ~TermInfosReader();
    TermInfosReader(Directory* dir, QString seg, FieldInfos* fis);
    void close();
    SegmentTermEnum* terms();
    int getSkipInterval();
     int getMaxSkipLevels();
private:
    Directory* directory;
    QString segment;
    FieldInfos* fieldInfos;
    SegmentTermEnum* origEnum;
    SegmentTermEnum* indexEnum;
    __int64 size;
    Term* indexTerms;//array
    int indexTermsLength;
    TermInfo* indexInfos;//array
    __int64* indexPointers;//array
};

#endif // TERMINFOSREADER_H
