#include "documentwriter.h"
#include "indexwriter.h"
#include "analyzer.h"
#include "directory.h"
#include "field.h"
#include "fieldinfos.h"
#include "document.h"
#include "posting.h"
#include "util.h"
#include <QVectorIterator>
#include "ICTCLAS30.h"
#include <QLibrary>
#include <string>
#include <iostream>
#include <QFile>
#include <QStringList>
#include "fieldswriter.h"
#include "terminfoswriter.h"
#include "similarity.h"
using namespace std;
QVector<QString>* DocumentWriter::STOPWORDS =  new QVector<QString>();
DocumentWriter::DocumentWriter()
{

}
DocumentWriter::~DocumentWriter()
{
    clearPostingTable();
    if(fieldInfos != 0) delete fieldInfos;fieldInfos = 0;
    DELETEARRAY(fieldLengths);
    DELETEARRAY(fieldPositions);
    DELETEARRAY(fieldBoosts);
   delete termBuffer;termBuffer = 0;
}
//
DocumentWriter::DocumentWriter(Directory* directory, Analyzer* analyzer, IndexWriter* writer)
    :termIndexInterval(IndexWriter::DEFAULT_TERM_INDEX_INTERVAL)
    ,fieldInfos(0),termBuffer(new Term("", ""))

{
    if(DocumentWriter::STOPWORDS->size() == 0)
    {
        QFile file("data/stopwords.txt");

                if(file.open(QIODevice::ReadOnly | QIODevice::Text))
                {
                    while(!file.atEnd())
                    {
                        QByteArray bytea = file.readLine();
                        //QString str(bytea);
                        QString str(QObject::tr(QString(bytea).toStdString().c_str()));
                        //std::cout << str.toStdString();
                        STOPWORDS->append(str.trimmed());
                    }
                }


                file.flush();
                file.close();
    }

    this->directory = directory;
    this->analyzer = analyzer;
    this->similarity = writer->getSimilarity();
    this->maxFieldLength = writer->getMaxFieldLength();
    this->termIndexInterval = writer->getTermIndexInterval();
  }
//
void DocumentWriter::addDocument(QString segment, Document *doc)
{
     fieldInfos = new FieldInfos();
     fieldInfos->add(doc);
     clearPostingTable();
     fieldLengths = new int[fieldInfos->size()];    // init fieldLengths
         fieldPositions = new int[fieldInfos->size()];  // init fieldPositions
         fieldOffsets = new int[fieldInfos->size()];    // init fieldOffsets
         fieldBoosts = new float[fieldInfos->size()];	  // init fieldBoosts
        for(int i = 0;i < fieldInfos->size();i++)
        {
            fieldLengths[i] = 0;
            fieldPositions[i] = 0;
            fieldOffsets[i] = 0;
            fieldBoosts[i] = doc->getBoost();
        }
      invertDocument(doc);//
      // sort postingTable into an array
      Posting** postings = sortPostingTable();
      // write field infos
      fieldInfos->write(directory, segment + QString(".fnm"));
      //write field values
      FieldsWriter fieldsWriter(directory, segment, fieldInfos);
      try
      {
          fieldsWriter.addDocument(doc);
      }catch(...)
      {
          fieldsWriter.close();
          throw;
      }
      fieldsWriter.close();

      // write postings
            writePostings(postings, segment);

            // write norms of indexed fields
            writeNorms(segment);

 }
//
void DocumentWriter::invertDocument(Document *doc)
{
    QVectorIterator<Field*> fieldIterator(doc->getFields());
     while (fieldIterator.hasNext())
    {
        Field* field = fieldIterator.next();
       QString fieldName = field->getname();
             int fieldNumber = fieldInfos->fieldNumber(fieldName);

             int length = fieldLengths[fieldNumber];     // length of field
             int position = fieldPositions[fieldNumber]; // position in field
             //if (length>0) position+=analyzer.getPositionIncrementGap(fieldName);
             int offset = fieldOffsets[fieldNumber];       // offset field
             if(field->getisIndexed())
             {
                 if (!field->getisTokenized())
                 {		  // un-tokenized field
                           QString stringValue = field->stringValue();
                             addPosition(fieldName, stringValue, position++);
                           offset += stringValue.length();
                           length++;
                 }
                 else
                 {
                    // tokenized field
                     QLibrary lib("ICTCLAS30.dll");
                     if(lib.load())
                     {
                         QString content = field->stringValue();
                         string aaa = content.toStdString();
                         const char* sInput =aaa.c_str();
                         field->setValue(QObject::tr(sInput));//set fieldvalue in right code
                         //初始化分词组件
                                 if(!ICTCLAS_Init())
                                 {
                                        cout << "ICTCLAS INIT FAILED!\n";
                                         return ;
                                 }

                                 ICTCLAS_SetPOSmap(ICT_POS_MAP_SECOND);

                                 const char *sResult = ICTCLAS_ParagraphProcess(sInput, 1);
                                 QString str(sResult);
                                  QStringList list1 = str.split(" ");
                                  foreach (QString str, list1)
                                  {
                                       string str1 = str.toStdString();
                                       string str2 = str1.substr(0,str1.find('/'));
                                       //cout << str2 << endl;
                                       QString stringbuf(QObject::tr(str2.c_str()));
                                       QString buf1 = stringbuf.trimmed().toLower();
                                       if(!STOPWORDS->contains(buf1) && (buf1 != ""))
                                           {
                                              addPosition(fieldName, buf1, position++);
                                           }
                                       if (++length >= maxFieldLength)
                                          {
                                               cout << "maxFieldLength " << maxFieldLength << " reached, ignoring following tokens\n";
                                               break;
                                          }
                                       offset += stringbuf.length();
                                  }


                         //free the resources of the word compontent
                         ICTCLAS_Exit();
                      }

                 }
                 fieldLengths[fieldNumber] = length;	  // save field length
                 fieldPositions[fieldNumber] = position;	  // save field position
                 fieldBoosts[fieldNumber] *= field->getBoost();
                 fieldOffsets[fieldNumber] = offset;
             }
    }
}
//
void DocumentWriter::clearPostingTable()
{
    if(postingTable.size() != 0)
    {
        QMapIterator<Term*,Posting*> i(postingTable);
        while (i.hasNext()) {
            i.next();
            Term* t = i.key();
            Posting* p = i.value();
           delete t; t = 0;
           delete p;p = 0;
        }
        postingTable.clear();
    }
}
//
void DocumentWriter::addPosition(QString field, QString text, int position)
{
    termBuffer->set(field, text);
    bool isHaveTerm = false;
    Term* tem;
    QMapIterator<Term*,Posting*> iter(postingTable);
     while (iter.hasNext())
    {
         iter.next();
         tem = iter.key();
         if(tem->compareTo(termBuffer)==0)
         {
             isHaveTerm = true;
             break;
         }
     }
    if(isHaveTerm)
    {
         Posting* ti = postingTable.value(tem);
         int freq = ti->freq;
         if(ti->positionsLength == freq)
         {
             int* newPositions = new int[freq * 2];	  // double size
             int* positions = ti->positions;
            for(int i = 0;i < freq;i++)
                newPositions[i] = positions[i];
            delete [] positions;
            positions = 0;
             ti->positions = newPositions;
             ti->positionsLength = freq * 2;
         }
         ti->positions[freq] = position;		  // add new position
         ti->freq = freq + 1;			  // update frequency
    }
    else
    {
        Term* term = new Term(field, text);
        postingTable.insert(term, new Posting(term, position));
    }

}
//sort the postingtable
Posting** DocumentWriter::sortPostingTable()
{
    Posting** array = new Posting*[postingTable.size()];
    int j = 0;
    QMapIterator<Term*,Posting*> i(postingTable);
    while (i.hasNext()) {
        i.next();
        array[j] = i.value();
        j++;
    }
    quickSort(array, 0, j - 1);
    return array;
}
//static
void DocumentWriter::quickSort(Posting**postings, int lo, int hi)
{
    if(lo >= hi) return;
    int mid = (lo + hi) / 2;
    if(postings[lo]->term->compareTo(postings[mid]->term) > 0)
    {
        Posting* tmp = postings[lo];
        postings[lo] = postings[mid];
        postings[mid] = tmp;
    }
    if (postings[mid]->term->compareTo(postings[hi]->term) > 0)
    {
          Posting* tmp = postings[mid];
          postings[mid] = postings[hi];
          postings[hi] = tmp;

          if (postings[lo]->term->compareTo(postings[mid]->term) > 0)
          {
            Posting* tmp2 = postings[lo];
            postings[lo] = postings[mid];
            postings[mid] = tmp2;
          }
     }

    int left = lo + 1;
        int right = hi - 1;

        if (left >= right)
          return;

        Term* partition = postings[mid]->term;

        for (; ;) {
          while (postings[right]->term->compareTo(partition) > 0)
            --right;

          while (left < right && postings[left]->term->compareTo(partition) <= 0)
            ++left;

          if (left < right) {
            Posting* tmp = postings[left];
            postings[left] = postings[right];
            postings[right] = tmp;
            --right;
          } else {
            break;
          }
        }

        quickSort(postings, lo, left);
        quickSort(postings, left + 1, hi);
}
//
void DocumentWriter::writePostings(Posting** postings, QString segment)
{
    IndexOutput* freq = 0;
    IndexOutput* prox = 0;
    TermInfosWriter* tis = 0;
    try
    {
        freq = directory->createOutput(segment + ".frq");
        prox = directory->createOutput(segment + ".prx");
        tis = new TermInfosWriter(directory, segment, fieldInfos,
                                        termIndexInterval);
         TermInfo* ti = new TermInfo();
         //QString currentField = null;
         int postingslength = postingTable.size();
         for (int i = 0; i < postingslength; i++)
         {
            Posting* posting = postings[i];
            ti->set(1, freq->getFilePointer(), prox->getFilePointer(), -1);
            tis->add(posting->term, ti);

            // add an entry to the freq file
                    int postingFreq = posting->freq;
                    if (postingFreq == 1)				  // optimize freq=1
                      freq->writeVInt(1);			  // set low bit of doc num.
                    else {
                      freq->writeVInt(0);			  // the document number
                      freq->writeVInt(postingFreq);			  // frequency in doc
                    }
                    int lastPosition = 0;
                    int* positions = posting->positions;
               for (int j = 0; j < postingFreq; j++)
               {
                    int position = positions[j];
                    int delta = position - lastPosition;
                    prox->writeVInt(delta);
                    lastPosition = position;
               }
         }
    }FINALLY(
            DOCLOSE(freq);
            DOCLOSE(prox);
            DOCLOSE(tis);
            );
}
//
 void DocumentWriter::writeNorms(QString segment)
 {

     for(int n = 0; n < fieldInfos->size(); n++)
     {
       FieldInfo* fi = fieldInfos->fieldInfo(n);
       if(fi->isIndexed && !fi->omitNorms)
       {
         float norm = fieldBoosts[n] * similarity->lengthNorm(fi->name, fieldLengths[n]);
         IndexOutput* norms = directory->createOutput(segment + ".f" + QString().setNum(n));
         try
         {
             norms->writeByte(Similarity::encodeNorm(norm));
         } FINALLY(
                 DOCLOSE(norms);
                 );
       }
     }

 }

