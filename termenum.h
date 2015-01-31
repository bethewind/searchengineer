#ifndef TERMENUM_H
#define TERMENUM_H

class Term;
class TermEnum
{
public:
    TermEnum();
    virtual ~TermEnum();
    virtual Term* term()=0;
    virtual bool next() = 0;
    virtual void close()=0;
};

#endif // TERMENUM_H
