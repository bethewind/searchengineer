#include "indexwriter.h"
#include "fsdirectory.h"
#include "segmentinfos.h"
#include "similarity.h"
#include "ramdirectory.h"
#include "segmentinfo.h"
#include "documentwriter.h"
#include <iostream>
#include <string>
#include "segmentmerger.h"
#include "indexreader.h"
#include "segmentreader.h"
#include "util.h"
#include <QFileInfoList>
#include <QFileInfo>
#include <QDir>
#include "indexfilenames.h"
using namespace std;
IndexWriter::IndexWriter()
{
}
//Constructs an IndexWriter for the index in path. Text will be analyzed with a.
//If create is true, then a new, empty index will be created in path, replacing
//the index already there, if any.
IndexWriter::IndexWriter(QString path, Analyzer* a, bool create)
    :segmentInfos(new SegmentInfos()),ramSegmentInfos(new SegmentInfos()),closeDir(false),closed(false)
    ,directory(0),analyzer(0)
    ,similarity(Similarity::getDefault()),ramDirectory(new RAMDirectory())
    ,termIndexInterval(IndexWriter::DEFAULT_TERM_INDEX_INTERVAL),maxFieldLength(IndexWriter::DEFAULT_MAX_FIELD_LENGTH)
    ,mergeFactor(IndexWriter::DEFAULT_MERGE_FACTOR)
    ,minMergeDocs(IndexWriter::DEFAULT_MAX_BUFFERED_DOCS)
    ,maxMergeDocs(IndexWriter::DEFAULT_MAX_MERGE_DOCS)
{
    init(FSDirectory::getDirectory(path), a, create,true);
}
//
void IndexWriter::init(Directory* d, Analyzer* a, const bool create,bool closeDir)
{
    this->closeDir = closeDir;
    this->directory = d;
    this->analyzer = a;

    if(create)
    {
        segmentInfos->write(directory);
    }
}
//Adds a document to this index.
void IndexWriter::addDocument(Document *doc)
{
    addDocument(doc,this->analyzer);
}
//Adds a document to this index, using the provided analyzer
void IndexWriter::addDocument(Document *doc, Analyzer *a)
{
    if(this->closed)
        std::cout << "the indexwriter is closed!" << std::endl;
     SegmentInfo* newSegmentInfo = buildSingleDocSegment(doc, analyzer);
    mutex.lock();
    ramSegmentInfos->append(newSegmentInfo);
    maybeFlushRamSegments();
    mutex.unlock();
 }
//
SegmentInfo* IndexWriter::buildSingleDocSegment(Document *doc, Analyzer *analyzer)
{
      DocumentWriter dw(ramDirectory, analyzer, this);
       QString segmentName = newRamSegmentName();
       dw.addDocument(segmentName,doc);
       SegmentInfo* si = new SegmentInfo(segmentName, 1, ramDirectory,false);
       si->setNumFields(dw.getNumFields());
       return si;

}
//
QString IndexWriter::newRamSegmentName()
{
    //string temstr = (QString("_ram_") + QString().setNum(ramSegmentInfos->counter++,36)).toStdString();
    return QString("_ram_") + QString().setNum(ramSegmentInfos->counter++,36);
}
//
void IndexWriter::maybeFlushRamSegments()
{
    if (ramSegmentInfos->size() >= minMergeDocs)
    {
         flushRamSegments(true);
         cout << "合并了4个文档!\n";
    }
}
//
void IndexWriter::flushRamSegments(bool triggerMerge)
  {
    if (ramSegmentInfos->size() > 0)
    {
      mergeSegments(ramSegmentInfos, 0, ramSegmentInfos->size());
      //if (triggerMerge) maybeMergeSegments(minMergeDocs);
    }
  }
//
 int IndexWriter::mergeSegments(SegmentInfos *sourceSegments, int minSegment, int end)
{
    bool doMerge = end > 0;
    QString mergedName = newSegmentName();
    SegmentMerger* merger = 0;
    QVector<SegmentInfo*> ramSegmentsToDelete;
    SegmentInfo* newSegment = 0;
    int mergedDocCount = 0;
    try
    {
        if(doMerge)
        {
             merger = new SegmentMerger(this, mergedName);
             for (int i = minSegment; i < end; i++)
             {
                 SegmentInfo* si = sourceSegments->info(i);
                 IndexReader* reader = SegmentReader::get(si, MERGE_READ_BUFFER_SIZE); // no need to set deleter (yet)
                 merger->add(reader);
                 if (reader->getDirectory() == this->ramDirectory)
                 {
                         ramSegmentsToDelete.append(si);
                 }
             }
        }
        //SegmentInfos* rollback = 0;
        bool success = false;
        // This is try/finally to rollback our internal state
        // if we hit exception when doing the merge:
         try
         {
             if(doMerge)
             {
                 mergedDocCount = merger->merge();
                 newSegment = new SegmentInfo(mergedName, mergedDocCount,directory,true);
             }
             if (sourceSegments != ramSegmentInfos)
             {
                       // Now save the SegmentInfo instances that
                       // we are replacing:
                       //rollback = (SegmentInfos) segmentInfos.clone();
             }
             if(doMerge)
             {

                 if (sourceSegments == ramSegmentInfos)
                 {
                   segmentInfos->append(newSegment);
                 } else
                 {
                   for (int i = end-1; i > minSegment; i--)     // remove old infos & add new
                   {
                       SegmentInfo* tem = sourceSegments->at(i);
                       if(tem != 0)
                       {
                            delete tem;
                       }
                       sourceSegments->replace(i,0);
                       sourceSegments->remove(i);
                   }

                   segmentInfos->replace(minSegment, newSegment);
                 }

             }
             deleteOldSeg();
              segmentInfos->write(directory);
              success = true;

          }FINALLY(
                  if (success)
                    {
                       // The non-ram-segments case is already committed
                      // (above), so all the remains for ram segments case
                       // is to clear the ram segments:
                       if (sourceSegments == ramSegmentInfos) {
                           ramSegmentInfos->removeAllElements();
                    }
                        } else {}
                  );
    }FINALLY(
                if(doMerge) merger->closeReaders();
            );
             return mergedDocCount;
}
//
QString IndexWriter::newSegmentName()
{
    //string temstr = (QString("_ram_") + QString().setNum(ramSegmentInfos->counter++,36)).toStdString();
    return QString("_") + QString().setNum(segmentInfos->counter++,36);
}
//Blindly delete the files used by the specific segments, with no reference
//counting and no retry. This is only currently used by writer to delete its
//RAM segments from a RAMDirectory.
void IndexWriter::deleteDirect(Directory *otherDir, QVector<SegmentInfo *>segments)
{
    int size = segments.size();
    for(int i=0;i<size;i++)
    {
      QVector<QString>* filestoDelete = segments.value(i)->getFiles();
      int size2 = filestoDelete->size();
      for(int j=0;j<size2;j++)
      {
        otherDir->deleteFile(filestoDelete->value(j));
      }
    }

}
void IndexWriter::deleteOldSeg()
{
    QString dir1 = this->directory->getDir();
    QDir dir(dir1);
    dir.setFilter(QDir::Files | QDir::Hidden | QDir::NoSymLinks);
    dir.setSorting(QDir::Size | QDir::Reversed);
    QFileInfoList list = dir.entryInfoList();
    for (int j = 0; j < list.size(); ++j)
    {
         QFileInfo fileInfo = list.at(j);
         QString fileName = fileInfo.fileName();
         if(fileName.startsWith(IndexFileNames::SEGMENTS) && (fileName != IndexFileNames::SEGMENTS_GEN))
         {
             QFile delFile(fileInfo.absoluteFilePath());
             delFile.remove();
             return;
         }
    }

}

