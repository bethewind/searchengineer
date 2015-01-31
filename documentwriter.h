#ifndef DOCUMENTWRITER_H
#define DOCUMENTWRITER_H
#include <QMap>
#include <QVector>
#include <QString>
#include "fieldinfos.h"
#include "term.h"
class Directory;
class Analyzer;
class IndexWriter;
class Document;
class Posting;
class Similarity;
class DocumentWriter
{
public:
    DocumentWriter();
    ~DocumentWriter();
    DocumentWriter(Directory* directory, Analyzer* analyzer, IndexWriter* writer);
    void addDocument(QString segment, Document* doc);
    void invertDocument(Document* doc);
    void  clearPostingTable();
    int getNumFields() {return this->fieldInfos->size();}

    static QVector<QString>* STOPWORDS;
private:
    Directory* directory;
    Analyzer* analyzer;
    Similarity* similarity;
    int maxFieldLength;
    IndexWriter* writer;
    int termIndexInterval;
    FieldInfos* fieldInfos;
    QMap<Term*,Posting*> postingTable;
    //QVector openTokenStreams;
    Term* termBuffer; // avoid consing
    int* fieldLengths;
     int* fieldPositions;
     int* fieldOffsets;
     float* fieldBoosts;
     //
     void addPosition(QString field, QString text, int position);
     Posting** sortPostingTable();
     static void quickSort(Posting** postings, int lo, int hi);
     void writePostings(Posting** postings, QString segment);
     void writeNorms(QString segment);
};

#endif // DOCUMENTWRITER_H
