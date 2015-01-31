#ifndef ANALYZER_H
#define ANALYZER_H
#include <QString>

class TokenStream;
class Reader;
class Analyzer
{
public:
    Analyzer();
    virtual ~Analyzer();
    virtual  TokenStream* tokenStream(QString fieldName, Reader* reader)=0;
};

#endif // ANALYZER_H
