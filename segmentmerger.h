#ifndef SEGMENTMERGER_H
#define SEGMENTMERGER_H
#include <QString>
#include "fieldinfos.h"
#include "segmentmergeinfo.h"
class Directory;
class IndexOutput;
class TermInfosWriter;
class SegmentMergeQueue;
class DefaultSkipListWriter;
class TermInfo;
class IndexWriter;
class IndexReader;
class SegmentMerger
{
public:
    SegmentMerger();
    SegmentMerger(IndexWriter* writer, QString name);
    void add(IndexReader* reader);
    void closeReaders();
    int merge();
    void deleteOldSeg();

    static const unsigned char NORMS_HEADER[4];
private:
    Directory* directory;
     QString segment;
    int termIndexInterval;
    QVector<IndexReader*> readers;
    IndexOutput* freqOutput;
    IndexOutput* proxOutput;
    TermInfosWriter* termInfosWriter;
    int skipInterval;
    int maxSkipLevels;
    SegmentMergeQueue* queue;
    DefaultSkipListWriter* skipListWriter;
    TermInfo* termInfo;
    int mergedDocs;
    FieldInfos* fieldInfos;

    int mergeFields();
    void mergeTerms();
    void mergeNorms();
    void mergeTermInfos();
    void mergeTermInfo(SegmentMergeInfo** smis, int n);
    int appendPostings(SegmentMergeInfo** smis, int n);
};

#endif // SEGMENTMERGER_H
