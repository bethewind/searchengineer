#ifndef FIELDSWRITER_H
#define FIELDSWRITER_H
#include <QString>

class Directory;
class FieldInfos;
class IndexOutput;
class Document;
class FieldsWriter
{
public:
    FieldsWriter();
    ~FieldsWriter();
    FieldsWriter(Directory* d, QString segment, FieldInfos* fn);
    void addDocument(Document* doc);
    void close();

    static const unsigned char FIELD_IS_TOKENIZED;
      static const unsigned char FIELD_IS_BINARY;
      static const unsigned char FIELD_IS_COMPRESSED;
private:
    FieldInfos* fieldInfos;
    IndexOutput* fieldsStream;
    IndexOutput* indexStream;
};

#endif // FIELDSWRITER_H
