#include "wvtdocumentinfo.h"

WVTDocumentInfo::WVTDocumentInfo(const QString& s,int i)
    :sourceName(""),classValue(-1),hasClassValue(true)
{
    sourceName = s;
    classValue = i;
}
