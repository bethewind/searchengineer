#include "segmentreader.h"
#include "segmentinfo.h"
#include "fieldinfos.h"
#include "fieldsreader.h"
#include <iostream>
#include "terminfosreader.h"
#include "directory.h"
#include "indexinput.h"
#include "segmentmerger.h"
#include "indexfilenames.h"
#include "util.h"
#include "segmenttermenum.h"
#include "segmenttermpositions.h"
#include "defaultsimilarity.h"
using namespace std;
SegmentReader::SegmentReader():singleNormStream(0),ones(0)
{
}
//
SegmentReader* SegmentReader::get(SegmentInfo* si, int readBufferSize)
{
     return get(si->dir, si, 0, false, false, readBufferSize);
}
//
SegmentReader* SegmentReader:: get(Directory* dir, SegmentInfo* si,
                                  SegmentInfos* sis,
                                  bool closeDir, bool ownDir,
                                  int readBufferSize)
 {
    SegmentReader* instance = new SegmentReader();
    instance->init(dir, sis, closeDir, ownDir);
    instance->initialize(si, readBufferSize);
    return instance;
  }
//
void SegmentReader::initialize(SegmentInfo *si, int readBufferSize)
{
    segment = si->name;
    this->si = si;
    bool success = false;
    try
    {
        Directory* cfsDir = this->getDirectory();
        // No compound file exists - use the multi-file format
        fieldInfos = new FieldInfos(cfsDir, segment + ".fnm");
        fieldsReader = new FieldsReader(cfsDir, segment, fieldInfos, readBufferSize);
        // Verify two sources of "maxDoc" agree:
         if (fieldsReader->getSize() != si->docCount)
         {
               cout << "fieldsreader.size differ from si.doccount";
         }
         tis = new TermInfosReader(cfsDir, segment, fieldInfos);
         freqStream = cfsDir->openInput(segment + ".frq");
         proxStream = cfsDir->openInput(segment + ".prx");
         openNorms(cfsDir);
          success = true;

    }FINALLY(
            if (!success)
            {
                doClose();
            }
            );
}
//
void SegmentReader::openNorms(Directory *cfsDir)
{

    __int64 nextNormSeek = sizeof(SegmentMerger::NORMS_HEADER)/sizeof(SegmentMerger::NORMS_HEADER[0]); //skip header (header unused for now)
    int maxDoc = si->docCount;
    for (int i = 0; i < fieldInfos->size(); i++)
    {
      FieldInfo* fi = fieldInfos->fieldInfo(i);
      if (fi->isIndexed && !fi->omitNorms) {
        Directory* d = this->getDirectory();
        QString fileName = si->getNormFileName(fi->number);

          d = cfsDir;


        // singleNormFile means multiple norms share this file
          bool singleNormFile = fileName.endsWith(QString(".") + IndexFileNames::NORMS_EXTENSION);
        IndexInput* normInput = 0;
        __int64 normSeek;

        if (singleNormFile) {
          normSeek = nextNormSeek;
          if (singleNormStream==0) {
            singleNormStream = d->openInput(fileName);
          }
          // All norms in the .nrm file can share a single IndexInput since
          // they are only used in a synchronized context.
          // If this were to change in the future, a clone could be done here.
          normInput = singleNormStream;
        } else {
           normSeek = 0;
           normInput = d->openInput(fileName);
        }

        norms.insert(fi->name, new SegmentReader::Norm(normInput, fi->number, normSeek));
        nextNormSeek += maxDoc; // increment also if some norms are separate
      }
    }

}
void SegmentReader::doClose()
{
    if (fieldsReader != 0) {
      fieldsReader->close();
    }
    if (tis != 0) {
      tis->close();
    }

    if (freqStream != 0)
      freqStream->close();
    if (proxStream != 0)
      proxStream->close();

    closeNorms();

  }
//
void SegmentReader::closeNorms()
{
    if(norms.size() != 0)
    {
        QMapIterator<QString,Norm*> i(norms);
        while (i.hasNext()) {
            i.next();
            Norm* p = i.value();
           p->close();
        }
        norms.clear();
    }
}
//get field count in this segment
FieldInfos* SegmentReader::getField()
{
    return this->fieldInfos;
}
//
bool SegmentReader::hasNorms(QString field)
{
    return norms.contains(field);
}

SegmentReader::Norm::Norm(IndexInput* in, int number, __int64 normSeek)
    :bytes(0)
  {
    this->in = in;
    this->number = number;
    this->normSeek = normSeek;
  }
SegmentReader::Norm::~Norm()
{
     DELETE(in);
    DELETEARRAY(this->bytes);
}
//
void SegmentReader::Norm::close()
{
   DOCLOSE(in);
}
//
int SegmentReader::maxDoc()
{
    return si->docCount;
}
Document* SegmentReader::document(int n)
{
    return fieldsReader->doc(n);
}
//
TermEnum* SegmentReader::terms()
{
    return this->tis->terms();
}
//
int SegmentReader::numDocs()
{
    return maxDoc();
}
//
TermPositions* SegmentReader::termPositions()
{
     return new SegmentTermPositions(this);
}
//
void SegmentReader::getNorms(QString field, unsigned char* bytes, int offset)
{

    Norm* norm = 0;
    if(this->hasNorms(field))
    {
        norm = norms.value(field);
    }
    if (norm == 0)
    {
        if(ones == 0)
            ones = new unsigned char[maxDoc()];

        for(int i = 0;i < maxDoc();i++)
        {
            ones[i] = DefaultSimilarity::encodeNorm(1.0f);
        }

        for(int j = 0;j < maxDoc();j++)
        {
            bytes[offset+j] = ones[j];
        }
      //System.arraycopy(fakeNorms(), 0, bytes, offset, maxDoc());
      return;
    }
//
    if (norm->bytes != 0)         // can copy from cache
    {
        for(int j = 0;j < maxDoc();j++)
        {
            bytes[offset+j] = norm->bytes[j];
        }
      //System.arraycopy(norm.bytes, 0, bytes, offset, maxDoc());
      return;
    }

    // Read from disk.  norm.in may be shared across  multiple norms and
    // should only be used in a synchronized context.
    norm->in->seek(norm->normSeek);
    norm->in->readBytes(bytes, offset, maxDoc());

}
