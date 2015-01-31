#ifndef TERMDOCS_H
#define TERMDOCS_H
#include "termenum.h"
class TermDocs
{
public:
    TermDocs();
    virtual ~TermDocs();
    virtual void close()=0;
    virtual void seek(TermEnum* termEnum)=0;
    // Moves to the next pair in the enumeration.  <p> Returns true iff there is
    //such a next pair in the enumeration.
     virtual bool next()=0;
     //Returns the current document number.
     virtual int getDoc()=0;
     virtual int getFreq()=0;
};

#endif // TERMDOCS_H
