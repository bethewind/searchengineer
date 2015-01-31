#ifndef WVTWORDVECTOR_H
#define WVTWORDVECTOR_H
#include <QVector>
class WVTDocumentInfo;
class WVTWordVector
{
private:
    double* values;
    WVTDocumentInfo* documentInfo;
public:
    WVTWordVector();
    ~WVTWordVector(){delete[] values;values =0;}
    void setDocumentInfo(WVTDocumentInfo* wvtdocumentinfo)
        {
            documentInfo = wvtdocumentinfo;
        }
    void setValues(double* value){values = value;}
    double* getValues(){return values;}
    WVTDocumentInfo* getDocumentInfo(){return documentInfo;}

};

#endif // WVTWORDVECTOR_H
