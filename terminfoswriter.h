#ifndef TERMINFOSWRITER_H
#define TERMINFOSWRITER_H
#include <QString>
#include "term.h"
#include "terminfo.h"
#include "indexoutput.h"

class Directory;
class FieldInfos;
class TermInfosWriter
{
public:
    TermInfosWriter();
    ~TermInfosWriter();
    TermInfosWriter(Directory* directory, QString segment, FieldInfos* fis,
                    int interval);
    TermInfosWriter(Directory* directory, QString segment, FieldInfos* fis,
                    int interval,bool isIndex);
    //
    void add(Term* term,TermInfo* ti);
    void close();

    int indexInterval;
    int skipInterval;
    int maxSkipLevels;
    static const int FORMAT = -3;
private:
    Term* lastTerm;
    TermInfo* lastTi;
    __int64 size;
    __int64 lastIndexPointer;
    bool isIndex;
    TermInfosWriter* other;
    IndexOutput* output;
    FieldInfos* fieldInfos;

    void initialize(Directory* directory, QString segment, FieldInfos* fis,
                              int interval, bool isi);
    void writeTerm(Term* term);
};

#endif // TERMINFOSWRITER_H
