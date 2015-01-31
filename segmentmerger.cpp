#include "segmentmerger.h"
#include "indexwriter.h"
#include "indexreader.h"
#include "fieldswriter.h"
#include "util.h"
#include "directory.h"
#include "terminfoswriter.h"
#include "indexoutput.h"
#include "defaultskiplistwriter.h"
#include "segmentmergequeue.h"
#include "segmentmergeinfo.h"
#include "indexfilenames.h"
#include <iostream>
#include <QFileInfoList>
#include <QFileInfo>
#include <QDir>
using namespace std;

const unsigned char SegmentMerger::NORMS_HEADER[4] ={'N','R','M',-1};
SegmentMerger::SegmentMerger()
{
}
//
SegmentMerger::SegmentMerger(IndexWriter* writer, QString name)
    :termIndexInterval(IndexWriter::DEFAULT_TERM_INDEX_INTERVAL)
    ,termInfo(new TermInfo())
{
    directory = writer->getDirectory();
    segment = name;
    termIndexInterval = writer->getTermIndexInterval();
 }
//
void SegmentMerger::add(IndexReader *reader)
{
    this->readers.append(reader);
}
//close all IndexReaders that have been added.
void SegmentMerger::closeReaders()
{
    for (int i = 0; i < readers.size(); i++)
    {  // close readers
      IndexReader* reader = readers.at(i);
      reader->close();
    }

}
//Merges the readers specified by the add method into the directory passed to the constructor
//Returns:The number of documents that were merged
int SegmentMerger::merge()
{
    mergedDocs = mergeFields();
    mergeTerms();
    mergeNorms();

    return mergedDocs;
}
//
int SegmentMerger::mergeFields()
{
    fieldInfos = new FieldInfos();
    int docCount = 0;
    for(int i = 0;i < readers.size();i++)
    {
        IndexReader* reader = readers.at(i);
        FieldInfos* fieldInfos1 = reader->getField();
        for(int j = 0;j < fieldInfos1->size();j++)
        {
             FieldInfo* fi = fieldInfos1->fieldInfo(j);
             fieldInfos->add(fi->name,fi->isIndexed,!reader->hasNorms(fi->name));
        }
    }
    fieldInfos->write(directory, segment + ".fnm");
    FieldsWriter* fieldsWriter = // merge field values
               new FieldsWriter(directory, segment, fieldInfos);
    try
    {
        for(int i = 0;i < readers.size();i++)
        {
            IndexReader* reader = readers.at(i);
            int maxDoc = reader->maxDoc();
            for(int j = 0;j < maxDoc;j++)
            {
                fieldsWriter->addDocument(reader->document(j));
                docCount++;
            }
        }
    }FINALLY(
            fieldsWriter->close();
            );
    return docCount;
}
//
void SegmentMerger::mergeTerms()
{
    try
    {
        freqOutput = directory->createOutput(segment + ".frq");
        proxOutput = directory->createOutput(segment + ".prx");
        termInfosWriter = new TermInfosWriter(directory, segment, fieldInfos,
                                          termIndexInterval);
        skipInterval = termInfosWriter->skipInterval;
        maxSkipLevels = termInfosWriter->maxSkipLevels;
        skipListWriter = new DefaultSkipListWriter(skipInterval, maxSkipLevels, mergedDocs, freqOutput, proxOutput);
        queue = new SegmentMergeQueue(readers.size());
        mergeTermInfos();
    }FINALLY
        (  
                DOCLOSE(freqOutput);
                DOCLOSE(proxOutput);
                DOCLOSE(termInfosWriter);
                DOCLOSE(queue);
        );
}
//
void SegmentMerger::mergeTermInfos()
{
    int base = 0;
    for(int i = 0;i <readers.size();i++)
    {
        IndexReader* reader = readers.at(i);
        TermEnum* termEnum = reader->terms();
        SegmentMergeInfo* smi = new SegmentMergeInfo(base, termEnum, reader);
        base += reader->numDocs();
        if(smi->next())
            queue->put(smi);
        else
            smi->close();
    }

    SegmentMergeInfo** match = new SegmentMergeInfo*[readers.size()];
    while(queue->getSize() > 0)
    {
        int matchSize = 0;
        match[matchSize++] = queue->pop();
        Term* term = match[0]->term;
        SegmentMergeInfo* top = queue->top();

        while (top != 0 && term->compareTo(top->term) == 0)
        {
            match[matchSize++] = queue->pop();
            top = queue->top();
        }

        mergeTermInfo(match, matchSize);		  // add new TermInfo
        while (matchSize > 0)
        {
             SegmentMergeInfo* smi = match[--matchSize];
             if (smi->next())
                queue->put(smi);			  // restore queue
             else
                smi->close();				  // done with a segment
        }
    }
}
//merge the same term in the smis array
void SegmentMerger::mergeTermInfo(SegmentMergeInfo**smis, int n)
{

    __int64 freqPointer = freqOutput->getFilePointer();
    __int64 proxPointer = proxOutput->getFilePointer();

    int df = appendPostings(smis, n);		  // append posting data
    __int64 skipPointer = skipListWriter->writeSkip(freqOutput);

    if (df > 0)
    {
      // add an entry to the dictionary with pointers to prox and freq files
      termInfo->set(df, freqPointer, proxPointer, (int) (skipPointer - freqPointer));
      termInfosWriter->add(smis[0]->term, termInfo);
    }

}
//Process postings from multiple segments all positioned on the same term. Writes
//out merged entriesinto freqOutput and the proxOutput streams.
int SegmentMerger::appendPostings(SegmentMergeInfo** smis, int n)
{
    int lastDoc = 0;
    int df = 0;  //number of doc contain the term
    this->skipListWriter->resetSkip();
    for(int i = 0;i < n;i++)
    {
        SegmentMergeInfo* smi = smis[i];
        TermPositions* postings = smi->getPositions();
        int base = smi->base;
        postings->seek(smi->termEnum);
        while(postings->next())
        {
            int doc = postings->getDoc();
            doc += base;
            if (doc < 0 || (df > 0 && doc <= lastDoc))
                std::cout << "docs out of order doc <= lastDoc";

                df++;
            if ((df % skipInterval) == 0)
                {
                    skipListWriter->setSkipData(lastDoc);
                    skipListWriter->bufferSkip(df);
                 }

              int docCode = (doc - lastDoc) << 1;	  // use low bit to flag freq=1
              lastDoc = doc;
              int freq = postings->getFreq();
              if (freq == 1)
               {
                   freqOutput->writeVInt(docCode | 1);	  // write doc & freq=1
               }
              else
              {
                  freqOutput->writeVInt(docCode);	  // write doc
                  freqOutput->writeVInt(freq);		  // write frequency in doc
              }

              int lastPosition = 0;
              for(int j = 0;j < freq;j++)
              {
                    int position = postings->nextPosition();
                    int delta = position - lastPosition;
                    this->proxOutput->writeVInt(delta);
                    lastPosition = position;
              }
        }
    }

    return df;
}
void SegmentMerger::mergeNorms()
{

    unsigned char* normBuffer = 0;
    int normBufferLength = 0;
    IndexOutput* output = 0;
    try {
      for (int i = 0; i < fieldInfos->size(); i++)
        {
        FieldInfo* fi = fieldInfos->fieldInfo(i);
        if (fi->isIndexed && !fi->omitNorms) {
          if (output == 0) {
              output = directory->createOutput(segment + "." + IndexFileNames::NORMS_EXTENSION);
            output->writeBytes((unsigned char*)NORMS_HEADER,sizeof(NORMS_HEADER)/sizeof(NORMS_HEADER[0]));
          }
          for (int j = 0; j < readers.size(); j++) {
            IndexReader* reader = readers.at(j);
            int maxDoc = reader->maxDoc();
            if (normBuffer == 0 || normBufferLength < maxDoc) {
              // the buffer is too small for the current segment
              normBuffer = new unsigned char[maxDoc];
              normBufferLength = maxDoc;
            }
            reader->getNorms(fi->name, normBuffer, 0);
              //optimized case for segments without deleted docs
              output->writeBytes(normBuffer, maxDoc);
          }
        }
      }
    } FINALLY (
      if (output != 0)
        {
        output->close();
      }
    );

}
//


