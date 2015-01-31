#include "wvtconfiguration.h"

WVTConfiguration::WVTConfiguration(const QString& s,int i)
    :sourceName(""),classValue(-1),hasClassValue(true)
{
    sourceName = s;
    classValue = i;
}
