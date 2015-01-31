#ifndef WVTOOL_H
#define WVTOOL_H
#include <QString>
#include <QVector>
class QFile;
class WVTFileInputList;
class WVTWordList;
class WVTWordVector;
class WVTool
{
private:
   static QVector<QString>* STOPWORDS;
public:
    WVTool();
    WVTWordList* createWordList(WVTFileInputList* wvtinputlist);
    void getKeyWords(const QString content,WVTWordList* wvtwordlist);
   static QString getFileValue(const QString& filePath);
    void createVectors(WVTFileInputList* wvtinputlist,WVTWordList* wvtwordlist,QFile& path);
    QString createVector(QString value,WVTWordList* wvtwordlist);
    void write(WVTWordVector* wvtwordvector,WVTWordList* wvtwordlist,QFile& path);
    QString getVector(WVTWordVector* wvtwordvector,WVTWordList* wvtwordlist);
};

#endif // WVTOOL_H
