#ifndef FIELDINFOS_H
#define FIELDINFOS_H
#include <QVector>
#include <QMap>
#include <QString>
#include "fieldinfo.h"
class Document;
class Directory;
class IndexOutput;
class IndexInput;
class FieldInfos
{
public:
    FieldInfos();
    ~FieldInfos();
    FieldInfos(Directory* d, QString name);
      static const unsigned char IS_INDEXED;
      static const unsigned char STORE_TERMVECTOR;
      static const unsigned char STORE_POSITIONS_WITH_TERMVECTOR;
      static const unsigned char STORE_OFFSET_WITH_TERMVECTOR;
      static const unsigned char OMIT_NORMS;
      static const unsigned char STORE_PAYLOADS;

      //
      void add(Document* doc);
      void add(QString name, bool isIndexed, bool omitNorms);
      FieldInfo* fieldInfo(QString fieldName);
      FieldInfo* fieldInfo(int fieldName);
      void addInternal(QString name, bool isIndexed, bool omitNorms);
      int size(){return byNumber.size();}
      int fieldNumber(QString fieldName);
      void write(Directory* d, QString name);
      void write(IndexOutput* output);
      void read(IndexInput* input);
      QString fieldName(int fieldNumber);
private:
      QVector<FieldInfo*> byNumber;
      QMap<QString,FieldInfo*> byName;
};

#endif // FIELDINFOS_H
