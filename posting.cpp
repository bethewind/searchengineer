#include "posting.h"
Posting::Posting()
{
}
Posting::~Posting()
{
    if(this->positions != 0)
    {
        delete[] this->positions;
        this->positions = 0;
    }
}
//
Posting::Posting(Term *t, int position)
{
       term = t;
       freq = 1;
       positions = new int[1];
       this->positionsLength = 1;
       positions[0] = position;
}
