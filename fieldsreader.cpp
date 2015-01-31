#include "fieldsreader.h"
#include "directory.h"
#include "indexinput.h"
#include "util.h"
#include "document.h"
#include "fieldinfos.h"
#include "field.h"
#include "fieldswriter.h"
FieldsReader::FieldsReader()
{
}
FieldsReader::~FieldsReader()
{
    DELETE(fieldsStream);
     DELETE(indexStream);
}
FieldsReader::FieldsReader(Directory *d, QString segment, FieldInfos *fn
                           , int readBufferSize)
{
       fieldInfos = fn;
       fieldsStream = d->openInput(segment + ".fdt");
       indexStream = d->openInput(segment + ".fdx");
       size = (int) (indexStream->getLength() / 8);
}
//
void FieldsReader::close()
{
    DOCLOSE(fieldsStream);
     DOCLOSE(indexStream);
}
//
Document* FieldsReader::doc(int n)
{
     indexStream->seek(n * 8L);
     __int64 position = indexStream->readLong();
     fieldsStream->seek(position);
     Document* doc = new Document();
     int numFields = fieldsStream->readVInt();
     for (int i = 0; i < numFields; i++)
      {
        int fieldNumber = fieldsStream->readVInt();
        FieldInfo* fi = fieldInfos->fieldInfo(fieldNumber);
        unsigned char bits = fieldsStream->readByte();
        bool compressed = (bits & FieldsWriter::FIELD_IS_COMPRESSED) != 0;
        bool tokenize = (bits & FieldsWriter::FIELD_IS_TOKENIZED) != 0;
        bool binary = (bits & FieldsWriter::FIELD_IS_BINARY) != 0;
        QString fvalue = fieldsStream->readString();
        Field* f = new Field(fi->name,fvalue,fi->isIndexed,tokenize
                             ,true,compressed,fi->omitNorms,binary);
                        doc->add(f);
       }

       return doc;
  }


