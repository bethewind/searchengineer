#ifndef WVTWORDLIST_H
#define WVTWORDLIST_H
#include <QVector>
#include <QString>
#include <QMap>
class WVTWord;
class WVTDocumentInfo;
class WVTWordList
{
private:
    int numClasses;
    bool appendWords;
    bool updateOnlyCurrent;
    int numDocuments; //total num of docements
    int ZNumDocuments;//num of documents which class is '+1'
    int numLocalTerms;//num of total Terms in the document
public:
    QVector<WVTWord*> wordList;
    QMap<QString,WVTWord*> wordMap;
    //
    WVTWordList(int i);
    WVTWordList(const QString& filePath);
    ~WVTWordList();
    void setAppendWords(bool flag){appendWords = flag;}
    void setUpdateOnlyCurrent(bool flag){updateOnlyCurrent = flag;}
    void addWordOccurance(const QString& s);
    void closeDocument(WVTDocumentInfo* wvtdocumentinfo);
    void pruneByFrequency(int i, int j);
    void store(const QString& path);
    int* getFrequenciesForCurrentDocument();
    int* getDocumentFrequencies();
    int getTermCountForCurrentDocument(){return numLocalTerms;}
    int getNumDocuments(){return numDocuments;}
};

#endif // WVTWORDLIST_H
