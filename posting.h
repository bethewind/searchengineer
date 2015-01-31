#ifndef POSTING_H
#define POSTING_H

#include "term.h"
class Posting
{
public:
    Posting();
     ~Posting();
    Posting(Term* t, int position);

    int freq;
    int *positions;				  // positions it occurs at
    int positionsLength;
    Term* term;
};

#endif // POSTING_H
