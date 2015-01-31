#include "segmentinfo.h"
#include "indexoutput.h"
#include <string>
#include "indexfilenames.h"
#include "directory.h"
#include "util.h"
using namespace std;
SegmentInfo::SegmentInfo() : docCount(0),delGen(SegmentInfo::NO)
        ,hasSingleNormFile(false),isCompoundFile(SegmentInfo::CHECK_DIR)
{
}
SegmentInfo::~SegmentInfo()
{
    DELETE(files);
    DELETEARRAY(normGen);
}
SegmentInfo::SegmentInfo(QString name, int docCount, Directory* dir,bool hasSingleNormFile)
{
    this->name = name;
    this->docCount = docCount;
    this->dir = dir;
    delGen = NO;
    this->hasSingleNormFile = hasSingleNormFile;
    normGen = 0;
    normGensize = 0;
    files = 0;
}
//
void SegmentInfo::write(IndexOutput* output)
   {
   output->writeString(name);
   output->writeInt(docCount);
   output->writeLong(delGen);
   output->writeByte((unsigned char) (hasSingleNormFile ? 1:0));
   if (normGensize == 0) {
     output->writeInt(NO);
   } else {
     output->writeInt(normGensize);
     for(int j = 0; j < normGensize; j++) {
       output->writeLong(normGen[j]);
     }
   }
   output->writeByte(isCompoundFile);
 }
//
void SegmentInfo::setNumFields(int numFields)
{
    if (normGen == 0)
    {
      normGen = new __int64[numFields];
      normGensize = numFields;
        for(int i=0;i<numFields;i++)
        {
          normGen[i] = NO;
        }

    }
}
//
QString SegmentInfo::getNormFileName(int number)
{

    QString prefix;

    __int64 gen;
    if (normGen == 0) {
      gen = CHECK_DIR;
    } else {
      gen = normGen[number];
    }

    if (hasSingleNormFile) {
      // case 2: lockless (or nrm file exists) - single file for all norms
        prefix = "." + IndexFileNames::NORMS_EXTENSION;
        return IndexFileNames::fileNameFromGeneration(name, prefix, WITHOUT_GEN);
    }

    // case 3: norm file for each field
    prefix = ".f";
    return IndexFileNames::fileNameFromGeneration(name, prefix + QString().setNum(number), WITHOUT_GEN);

}
//
QVector<QString>* SegmentInfo::getFiles()
{
    if(files != 0){return files;}

    files = new QVector<QString>();
    int length = sizeof(IndexFileNames::INDEX_EXTENSIONS_IN_COMPOUND_FILE)
                 /sizeof(IndexFileNames::INDEX_EXTENSIONS_IN_COMPOUND_FILE[0]);
    for (int i = 0; i < length; i++)
    {
        QString ext = IndexFileNames::INDEX_EXTENSIONS_IN_COMPOUND_FILE[i];
      QString fileName = name + "." + ext;
      if (dir->fileExists(fileName))
      {
        files->append(fileName);
      }
    }

    if(normGen != 0)
    {
        int length = normGensize;
        for(int i = 0;i < length;i++)
        {
            __int64 gen = normGen[i];
            if(gen == NO)
            {
                if(!this->hasSingleNormFile)
                {

                    QString fileName = name + "." + IndexFileNames::PLAIN_NORMS_EXTENSION
                                       + QString().setNum(i);
                    if (dir->fileExists(fileName)) {
                      files->append(fileName);
                    }

                }
            }
        }
    }

    return files;

}
