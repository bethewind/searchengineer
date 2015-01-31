#ifndef TFIDF_H
#define TFIDF_H
class WVTWordList;
class WVTDocumentInfo;
class WVTWordVector;
class TFIDF
{
public:
    TFIDF();
    WVTWordVector* createVector(int* ai, int i, WVTWordList* wvtwordlist, WVTDocumentInfo* wvtdocumentinfo);
};

#endif // TFIDF_H
