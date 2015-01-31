#ifndef SEGMENTREADER_H
#define SEGMENTREADER_H
#include "indexreader.h"
#include <QString>
#include <QMap>
#include "termenum.h"

class SegmentInfo;
class SegmentInfos;
class Directory;
class FieldInfos;
class FieldsReader;
class TermInfosReader;
class IndexInput;
class SegmentReader : public IndexReader
{
    class Norm
    {
        friend class SegmentReader;
    private:
        IndexInput* in;
        unsigned char* bytes;
        bool dirty;
        int number;
        __int64 normSeek;
        bool rollbackDirty;
    public:
        Norm(IndexInput* in, int number, __int64 normSeek);
        ~Norm();
        void close();

    };
public:
    SegmentReader();
    static SegmentReader* get(SegmentInfo* si, int readBufferSize);
    static SegmentReader* get(Directory* dir, SegmentInfo* si,
                                      SegmentInfos* sis,
                                      bool closeDir, bool ownDir,
                                      int readBufferSize);
    void initialize(SegmentInfo* si, int readBufferSize);
    void closeNorms();
    FieldInfos* getField();
    bool hasNorms(QString field);
    int maxDoc();
    Document* document(int n);
    TermEnum* terms();
    int numDocs();
    TermPositions* termPositions();
    void getNorms(QString field, unsigned char* bytes, int offset);

    FieldInfos* fieldInfos;
    TermInfosReader* tis;
    IndexInput* freqStream;
    IndexInput* proxStream;
private:
    SegmentInfo* si;
    FieldsReader* fieldsReader;
    QString segment;
    IndexInput* singleNormStream;
    QMap<QString,Norm*> norms;
    unsigned char* ones;

    void openNorms(Directory* cfsDir);
protected:
    void doClose();
};

#endif // SEGMENTREADER_H
