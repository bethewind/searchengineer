#ifndef WVTDOCUMENTINFO_H
#define WVTDOCUMENTINFO_H
#include <QString>
class WVTDocumentInfo
{
private:
    QString sourceName;
    int classValue;
    bool hasClassValue;
public:
    WVTDocumentInfo(const QString& s,int i);
    QString getSourceName(){return sourceName;}
    int getClassValue(){return classValue;}
};

#endif // WVTDOCUMENTINFO_H
