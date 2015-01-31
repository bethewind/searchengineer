#ifndef SIMILARITY_H
#define SIMILARITY_H
#include <QString>
class Similarity
{
public:
    Similarity();
    virtual ~Similarity();
    static Similarity* getDefault() {return Similarity::defaultImpl;}
    virtual float lengthNorm(QString fieldName, int numTokens)=0;
    static unsigned char encodeNorm(float f);
    static unsigned char floatToByte315(float f);
   static int floatToIntBits(float value);
private:
    static Similarity* defaultImpl;
};

#endif // SIMILARITY_H
