#include "fieldinfos.h"
#include "document.h"
#include "field.h"
#include "directory.h"
#include "indexoutput.h"
#include <string>
#include "indexinput.h"
#include "util.h"
using namespace std;
const unsigned char FieldInfos::IS_INDEXED = 0x1;
const unsigned char FieldInfos::STORE_TERMVECTOR = 0x2;
const unsigned char FieldInfos::STORE_POSITIONS_WITH_TERMVECTOR = 0x4;
const unsigned char FieldInfos::STORE_OFFSET_WITH_TERMVECTOR = 0x8;
const unsigned char FieldInfos::OMIT_NORMS = 0x10;
const unsigned char FieldInfos::STORE_PAYLOADS = 0x20;
FieldInfos::FieldInfos()
{
}
FieldInfos::FieldInfos(Directory *d, QString name)
{
    IndexInput* input = d->openInput(name);
    try
    {
        read(input);
    }FINALLY(
            input->close();
            );

}
FieldInfos::~FieldInfos()
{
    for(int i = 0;i < this->byNumber.size();i++)
    {
       if(byNumber[i] != 0)
       {
           delete byNumber[i];
           byNumber[i] = 0;
       }
    }
//    if(byName.size() != 0)
//    {
//        QMapIterator<QString,FieldInfo*> i(byName);
//        while (i.hasNext()) {
//            i.next();
//           FieldInfo* p = i.value();
//           delete p;p = 0;
//        }
//        byName.clear();
//    }
}
//
void FieldInfos::add(Document *doc)
{
    QVector<Field*> fields = doc->getFields();
    for(int i = 0;i < fields.size();i++)
    {
        add(fields[i]->getname(),fields[i]->getisIndexed(),fields[i]->getOmitNorms());
    }
}
//
void FieldInfos::add(QString name, bool isIndexed, bool omitNorms)
{
     FieldInfo* fi = fieldInfo(name);
     if(fi == 0)
     {
        addInternal(name,isIndexed,omitNorms);
     }
     else
     {
           if (fi->isIndexed != isIndexed)
           {
             fi->isIndexed = true;                      // once indexed, always index
           }
           if (fi->omitNorms != omitNorms)
           {
                   fi->omitNorms = false;                // once norms are stored, always store
           }
     }
     //return fi;
}
//
FieldInfo* FieldInfos::fieldInfo(QString fieldName)
{
    if(this->byName.contains(fieldName))
    {
        return this->byName.value(fieldName);
    }
    else
        return 0;
}
//
FieldInfo* FieldInfos::fieldInfo(int fieldNumber)
{
    if(fieldNumber < this->byNumber.size())
    {
        return byNumber[fieldNumber];
    }
    else
        return 0;
}
//
void FieldInfos::addInternal(QString name, bool isIndexed, bool omitNorms)
{
    FieldInfo* fi = new FieldInfo(name,isIndexed,byNumber.size(),omitNorms);
    byNumber.append(fi);
    byName.insert(name,fi);
    //return fi;
}
//
int FieldInfos::fieldNumber(QString fieldName)
{
    FieldInfo* fi = fieldInfo(fieldName);
    if(fi != 0)
        return fi->number;
    return -1;
}
//
void FieldInfos::write(Directory *d, QString name)
{
    IndexOutput* output = d->createOutput(name);
    try
    {
        write(output);
    }
    catch(...)
    {
        output->close();
        throw;
    }
    output->close();
    delete output;
    output = 0;
}
//
void FieldInfos::write(IndexOutput* output)
 {
    output->writeVInt(size());
    for (int i = 0; i < size(); i++)
    {
      FieldInfo* fi = fieldInfo(i);
      unsigned char bits = 0x0;
      if (fi->isIndexed) bits |= IS_INDEXED;
      //if (fi->storeTermVector) bits |= STORE_TERMVECTOR;
      //if (fi->storePositionWithTermVector) bits |= STORE_POSITIONS_WITH_TERMVECTOR;
      //if (fi->storeOffsetWithTermVector) bits |= STORE_OFFSET_WITH_TERMVECTOR;
      if (fi->omitNorms) bits |= OMIT_NORMS;
     // if (fi->storePayloads) bits |= STORE_PAYLOADS;

      output->writeString(fi->name);
      output->writeByte(bits);
    }
  }
//
void FieldInfos::read(IndexInput* input)
{
    int size = input->readVInt();//read in the size
    for (int i = 0; i < size; i++) {
      QString name = input->readString();
      unsigned char bits = input->readByte();
      bool isIndexed = (bits & IS_INDEXED) != 0;
      //bool storeTermVector = (bits & STORE_TERMVECTOR) != 0;
      //bool storePositionsWithTermVector = (bits & STORE_POSITIONS_WITH_TERMVECTOR) != 0;
      //bool storeOffsetWithTermVector = (bits & STORE_OFFSET_WITH_TERMVECTOR) != 0;
      bool omitNorms = (bits & OMIT_NORMS) != 0;
      //bool storePayloads = (bits & STORE_PAYLOADS) != 0;

      addInternal(name, isIndexed,omitNorms);
    }
 }
//
QString FieldInfos::fieldName(int fieldNumber)
{
     return fieldInfo(fieldNumber)->name;
}
