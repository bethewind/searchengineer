#ifndef WVTCONFIGURATION_H
#define WVTCONFIGURATION_H
#include <QString>
class WVTConfiguration
{
private:
    QString sourceName;
    int classValue;
    bool hasClassValue;
public:
    WVTConfiguration(const QString& s,int i);
};

#endif // WVTCONFIGURATION_H
