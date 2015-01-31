#ifndef FIELDINFO_H
#define FIELDINFO_H
#include <QString>
class FieldInfo
{
public:
    FieldInfo();
    FieldInfo(QString na, bool tk, int nu, bool omitNorms);
    //
    QString name;
    bool isIndexed;
    int number;
    bool omitNorms;
};

#endif // FIELDINFO_H
