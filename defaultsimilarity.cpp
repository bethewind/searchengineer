#include "defaultsimilarity.h"
#include <cmath>
DefaultSimilarity::DefaultSimilarity()
{
}
float DefaultSimilarity::lengthNorm(QString fieldName, int numTerms)
{
    return (float)(1.0 / sqrt(numTerms));
}
