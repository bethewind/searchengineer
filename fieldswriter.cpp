#include "fieldswriter.h"
#include "indexoutput.h"
#include "directory.h"
#include <QVectorIterator>
#include "document.h"
#include "field.h"
#include "fieldinfos.h"

const unsigned char FieldsWriter::FIELD_IS_TOKENIZED = 0x1;
const unsigned char FieldsWriter::FIELD_IS_BINARY = 0x2;
const unsigned char FieldsWriter::FIELD_IS_COMPRESSED = 0x4;
FieldsWriter::FieldsWriter()
{
}
FieldsWriter::~FieldsWriter()
{
    if(fieldsStream!=0){delete fieldsStream;fieldsStream = 0;}
    if(indexStream!=0){delete indexStream;indexStream = 0;}
}
 FieldsWriter::FieldsWriter(Directory* d, QString segment, FieldInfos* fn)
 {
    this->fieldInfos = fn;
    this->fieldsStream = d->createOutput(segment + QString(".fdt"));
    this->indexStream = d->createOutput(segment + QString(".fdx"));
 }
 //
 void FieldsWriter::addDocument(Document *doc)
 {
    indexStream->writeLong(fieldsStream->getFilePointer());
    int storedCount = 0;
    QVectorIterator<Field*> fieldIterator(doc->getFields());
    while (fieldIterator.hasNext())
    {
        Field* field = fieldIterator.next();
        if (field->getisStored())
              storedCount++;
    }
    fieldsStream->writeVInt(storedCount);
    QVectorIterator<Field*> fieldIterator1(doc->getFields());
    while (fieldIterator1.hasNext())
    {
        Field* field = fieldIterator1.next();
        if (field->getisStored())
        {

            fieldsStream->writeVInt(fieldInfos->fieldNumber(field->getname()));

            unsigned char bits = 0;
            if (field->getisTokenized())
                bits |= FieldsWriter::FIELD_IS_TOKENIZED;
//            if (field->ge)
//                bits |= FieldsWriter.FIELD_IS_BINARY;
//            if (field.isCompressed())
//                bits |= FieldsWriter.FIELD_IS_COMPRESSED;

            fieldsStream->writeByte(bits);
            fieldsStream->writeString(field->stringValue());

         }

     }

 }
 //
 void FieldsWriter::close()
 {
    fieldsStream->close();
    indexStream->close();
 }
