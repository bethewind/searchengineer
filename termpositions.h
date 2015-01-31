#ifndef TERMPOSITIONS_H
#define TERMPOSITIONS_H
#include "termdocs.h"
class TermPositions : public virtual TermDocs
{
public:
    TermPositions();
    virtual int nextPosition()=0;
};

#endif // TERMPOSITIONS_H
