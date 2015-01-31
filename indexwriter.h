#ifndef INDEXWRITER_H
#define INDEXWRITER_H
#include <QString>
#include "segmentinfo.h"
#include <QMutex>

class Analyzer;
class Directory;
class SegmentInfos;
class Similarity;
class RAMDirectory;
class Document;
class IndexWriter
{
public:
    IndexWriter();
    IndexWriter(QString path, Analyzer* a, bool create);
    void addDocument(Document* doc);
    void addDocument(Document* doc,Analyzer* a);
    SegmentInfo* buildSingleDocSegment(Document* doc, Analyzer* analyzer);
    QString newRamSegmentName();
    QString newSegmentName();
    Similarity* getSimilarity() {return this->similarity;}
    int getMaxFieldLength(){return this->maxFieldLength;}
    int getTermIndexInterval(){return this->termIndexInterval;}
    Directory* getDirectory(){return this->directory;}
    void deleteDirect(Directory* otherDir, QVector<SegmentInfo*> segments);
    void deleteOldSeg();
    //
    SegmentInfos* segmentInfos;
    SegmentInfos* ramSegmentInfos;   // the segments in ramDirectory
    QVector<SegmentInfo*> ramSegmentsToDelete;
    void maybeFlushRamSegments();
    void flushRamSegments(bool triggerMerge);
     static const int DEFAULT_TERM_INDEX_INTERVAL = 3;
      static const int DEFAULT_MAX_FIELD_LENGTH = 10000;
       static const int DEFAULT_MERGE_FACTOR = 2;
       static const int DEFAULT_MAX_BUFFERED_DOCS = 100;
       static const int DEFAULT_MAX_MERGE_DOCS = 2147483647;
private:
    bool closeDir;
    bool closed;
    Directory* directory;
    Analyzer* analyzer;
    Similarity* similarity;
    RAMDirectory* ramDirectory;
    QMutex mutex;

    int termIndexInterval;

    int maxFieldLength;

    int mergeFactor;

    int minMergeDocs;

    int maxMergeDocs;
    // The normal read buffer size defaults to 1024, but
    // increasing this during merging seems to yield
    // performance gains.  However we don't want to increase
    // it too much because there are quite a few
    // BufferedIndexInputs created during merging.  See
    // LUCENE-888 for details.
    static const int MERGE_READ_BUFFER_SIZE = 4096;

    void init(Directory* d, Analyzer* a, const bool create,bool closeDir);
    int mergeSegments(SegmentInfos* sourceSegments, int minSegment, int end);
};

#endif // INDEXWRITER_H
