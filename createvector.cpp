#include "createvector.h"
#include "wvtool.h"
#include "wvtfileinputlist.h"
#include "wvtdocumentinfo.h"
#include "wvtwordlist.h"
#include <QFile>
CreateVector::CreateVector()
{
}
//create vectors for the two classes
void CreateVector::wvToolVector (QString parth1,QString parth2,QString wvlist,QString wv)
{
    WVTool wvt;
    WVTFileInputList list(2);
    list.addEntry(new WVTDocumentInfo(parth1,+1));
    list.addEntry(new WVTDocumentInfo(parth2,-1));
    WVTWordList* wordList = wvt.createWordList(&list);
   wordList->pruneByFrequency(1,1000);
    wordList->store(wvlist);
    QFile file(wv);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text))
        wvt.createVectors(&list,wordList,file);
    file.flush();
    file.close();
    delete wordList;

}
