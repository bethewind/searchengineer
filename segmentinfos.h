#ifndef SEGMENTINFOS_H
#define SEGMENTINFOS_H
#include <QVector>
#include <QString>

class SegmentInfo;
class Directory;
class SegmentInfos : public QVector<SegmentInfo*>
{
public:
    SegmentInfos();
    void write(Directory* directory);
    QString getNextSegmentFileName();
    SegmentInfo* info(int i);
    void removeAllElements();

    static const int FORMAT = -1;//The file format version, a negative number.
    static const int FORMAT_LOCKLESS = -2;
    static const int FORMAT_SINGLE_NORM_FILE = -3;//This format adds a "hasSingleNormFile" flag into each segment info
    static const int CURRENT_FORMAT = FORMAT_SINGLE_NORM_FILE;
    int counter;
private:
    __int64 generation;
    __int64 lastGeneration;
    __int64 version;
};

#endif // SEGMENTINFOS_H
