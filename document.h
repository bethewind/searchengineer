#ifndef DOCUMENT_H
#define DOCUMENT_H
#include <QVector>

class Field;
class Document
{
public:
    Document();

    QVector<Field*> fields;
    void add(Field* field);
    QVector<Field*> getFields(){return fields;}
    float getBoost() {return boost;}
private:
    float boost;
};

#endif // DOCUMENT_H
