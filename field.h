#ifndef FIELD_H
#define FIELD_H
#include <QString>
class Field
{
public:
    Field();
    Field(QString name, QString value, QString store, QString index);
    Field(QString name, QString value,bool isIndexed,bool isTokenized
                 ,bool isStored,bool isCompressed,bool omitNorms,bool isBinary);
    QString getname()    { return name; }
    bool  getisIndexed()   { return isIndexed; }
     bool getOmitNorms() { return omitNorms; }
      bool  getisTokenized()   { return isTokenized; }
    QString stringValue()   { return fieldsData;}
    void setValue(const QString& value){this->fieldsData = value;}
    float getBoost(){return boost;}
    bool getisStored(){return isStored;}
    //store
    static const QString StoreCOMPRESS;
     static const QString StoreYES;
      static const QString StoreNO;
      //index
     static const QString IndexNO;
     static const QString IndexTOKENIZED;
     static const QString IndexUN_TOKENIZED;
     static const QString IndexNO_NORMS;
private:
     QString name;
     QString fieldsData;
     bool isStored;
     bool isIndexed;
     bool isTokenized;
     bool isBinary;
     bool isCompressed;
     bool lazy;
     float boost;
     bool omitNorms;
};

#endif // FIELD_H
