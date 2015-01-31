#include "field.h"
#include <iostream>
const QString Field::StoreCOMPRESS = "StoreCOMPRESS";
const QString Field::StoreYES= "StoreYES";
const QString Field::StoreNO= "StoreNO";
const QString Field::IndexNO= "IndexNO";
const QString Field::IndexTOKENIZED= "IndexTOKENIZED";
const QString Field::IndexUN_TOKENIZED= "IndexUN_TOKENIZED";
const QString Field::IndexNO_NORMS= "Index NO_NORMS";
Field::Field()
{
}
//
Field::Field(QString name, QString value, QString store, QString index)
    :isStored(false),isIndexed(true),isTokenized(true)
    ,isBinary(false),isCompressed(false),lazy(false),boost(1.0f)
    ,omitNorms(false)
{
    this->name = name;
    this->fieldsData = value;
    if (store == StoreYES){
          this->isStored = true;
          this->isCompressed = false;
        }
        else if (store == StoreCOMPRESS) {
          this->isStored = true;
          this->isCompressed = true;
        }
        else if (store == StoreNO){
          this->isStored = false;
          this->isCompressed = false;
        }
        else
            std::cout << "unknown store parameter"+store.toStdString() << std::endl;

        if (index == IndexNO) {
          this->isIndexed = false;
          this->isTokenized = false;
        } else if (index == IndexTOKENIZED) {
          this->isIndexed = true;
          this->isTokenized = true;
        } else if (index == IndexUN_TOKENIZED) {
          this->isIndexed = true;
          this->isTokenized = false;
        } else if (index == IndexNO_NORMS) {
          this->isIndexed = true;
          this->isTokenized = false;
          this->omitNorms = true;
        } else {
          std::cout << "unknown index parameter"+index.toStdString() << std::endl;
        }

        this->isBinary = false;

}
//
Field::Field(QString name, QString value,bool isIndexed,bool isTokenized
             ,bool isStored,bool isCompressed,bool omitNorms,bool isBinary)
{
    this->name = name;
    this->fieldsData = value;
    this->isBinary  = isBinary;
    this->isCompressed = isCompressed;
    this->isIndexed = isIndexed;
    this->isStored = isStored;
    this->isTokenized = isTokenized;
}
