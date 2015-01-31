#ifndef SEGMENTINFO_H
#define SEGMENTINFO_H
#include <QString>
#include <QVector>
class IndexOutput;
class Directory;
class SegmentInfo
{
public:
    SegmentInfo();
    ~SegmentInfo();
    SegmentInfo(QString name, int docCount, Directory* dir, bool hasSingleNormFile);
    void write(IndexOutput* output);
    void setNumFields(int numFields);
    QString getNormFileName(int number);
    QVector<QString>* getFiles();

    QString name;
    int docCount;
    static const int NO = -1;
    static const int YES = 1;
    static const int CHECK_DIR = 0;
     static const int WITHOUT_GEN = 0;
    Directory* dir;
private:
    __int64 delGen;
     bool hasSingleNormFile;
     __int64* normGen;//array
     int normGensize;
     unsigned char isCompoundFile;
     QVector<QString>* files;                             // cached list of files that this segment uses
                                                      // in the Directory
};

#endif // SEGMENTINFO_H
