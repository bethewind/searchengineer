#ifndef WVTWORD_H
#define WVTWORD_H
#include <QVector>
#include <QString>
class WVTWord
{
 private:
    const QString word;
    QVector<int> classCount;
    int documentCount;
    int zDocumentCount;
    int localCount;
 public:
    WVTWord(const QString& s, int i);
    void closeDocument(int i, bool flag);
    int getClassFrequency(int i);
    void addOccurance(){localCount++;}
    int getDocumentFrequency(){return documentCount;}
    int getLocalFrequency(){return localCount;}
    QString getWord(){return word;}
    void setDocumentFrequency(int i){documentCount = i;}
    void setClassFrequency(int i, int j);
};

#endif // WVTWORD_H
