#ifndef TERMBUFFER_H
#define TERMBUFFER_H
#include <QString>
#include "term.h"
#include "indexinput.h"

class FieldInfos;
class TermBuffer
{
public:
    TermBuffer();
    TermBuffer(const TermBuffer& clone);
    QString getText(){return text;}
    QString getField(){return field;}
    //int getTextLength(){return textLength;}
    Term* getTerm(){return term;}
    Term* toTerm();
    void reset() ;
    void set(TermBuffer* other);
    void read(IndexInput* input, FieldInfos* fieldInfos);
private:
    //static const char[] NO_CHARS = new char[0];
    QString field;
    QString text;
    //int textLength;
    Term* term;
};

#endif // TERMBUFFER_H
