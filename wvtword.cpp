#include "wvtword.h"

WVTWord::WVTWord(const QString& s, int i)
    :word(s),documentCount(0),zDocumentCount(0),localCount(0)
{
    classCount.resize(i);
    for(int j = 0; j < i; j++)
                classCount[j] = 0;
}
void WVTWord::closeDocument(int i, bool flag)
    {
        if(localCount > 0 && !flag)
        {
            documentCount++;
            if(classCount.size() != 0 && i < classCount.size())
            {
                if(i > 0)
                     classCount[0]++;
                else
                     classCount[1]++;
            }
            localCount = 0;
            if(i > 0)
            {
                zDocumentCount += 1;
            }
        } else
        {
            localCount = 0;
        }
    }
int WVTWord::getClassFrequency(int i)
    {
        if(classCount.size() != 0)
        {
            if(i > 0)
                return classCount[0];
            else
                return classCount[1];
        } else
        {
            return 0;
        }
    }
//
void WVTWord::setClassFrequency(int i, int j)
{
    if(i >= 0 && i < classCount.size())
        classCount[i] = j;
}
