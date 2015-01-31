#ifndef CHINESEANALYZER_H
#define CHINESEANALYZER_H
#include "analyzer.h"
class ChineseAnalyzer : public Analyzer
{
public:
    ChineseAnalyzer();
    TokenStream* tokenStream(QString fieldName, Reader* reader);
};

#endif // CHINESEANALYZER_H
