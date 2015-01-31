#ifndef FIELDSREADER_H
#define FIELDSREADER_H
#include <QString>

class Directory;
class FieldInfos;
class IndexInput;
class Document;
class FieldsReader
{
public:
    FieldsReader();
    ~FieldsReader();
    FieldsReader(Directory* d, QString segment, FieldInfos* fn, int readBufferSize);
    void close();
    int getSize(){return size;}
    Document* doc(int n);
private:
    FieldInfos* fieldInfos;
    IndexInput* fieldsStream;
    IndexInput* indexStream;
    int size;
};

#endif // FIELDSREADER_H
