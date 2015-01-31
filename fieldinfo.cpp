#include "fieldinfo.h"

FieldInfo::FieldInfo()
{
}
//
FieldInfo::FieldInfo(QString na, bool tk, int nu, bool omitNorms)
{
    name = na;
    isIndexed = tk;
    number = nu;
    this->omitNorms = omitNorms;
}
