#ifndef DEFAULTSIMILARITY_H
#define DEFAULTSIMILARITY_H
#include "similarity.h"
#include <QString>
class DefaultSimilarity : public Similarity
{
public:
    DefaultSimilarity();
    float lengthNorm(QString fieldName, int numTerms);
};

#endif // DEFAULTSIMILARITY_H
