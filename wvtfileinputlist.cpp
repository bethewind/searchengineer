#include "wvtfileinputlist.h"
#include <QDir>
#include "wvtdocumentinfo.h"
WVTFileInputList::WVTFileInputList(int i)
{
     numClasses = i;
}
WVTFileInputList::~WVTFileInputList()
{
    for(int i = 0;i < inputList.size();i++)
    {
        delete inputList[i];
        inputList[i] = 0;
    }
    for(int i = 0;i < fileList.size();i++)
    {
        delete fileList[i];
        fileList[i] = 0;
    }
}
void WVTFileInputList::addEntry(WVTDocumentInfo* wvtdocumentinfo)
    {
        inputList.append(wvtdocumentinfo);
    }
void WVTFileInputList::getEntries()
{
   for(int i = 0;i < inputList.size();i++)
   {
       QDir dir(inputList[i]->getSourceName());
       dir.setFilter(QDir::Files | QDir::Hidden | QDir::NoSymLinks);
       dir.setSorting(QDir::Size | QDir::Reversed);
       QFileInfoList list = dir.entryInfoList();
               for (int j = 0; j < list.size(); ++j) {
                   QFileInfo fileInfo = list.at(j);
                   QString abpath = fileInfo.absoluteFilePath();
                   int classvalues = inputList[i]->getClassValue();
                   WVTDocumentInfo* wvtdocmentinfo = new WVTDocumentInfo(abpath,classvalues);
                   fileList.append(wvtdocmentinfo);
               }

   }
}
