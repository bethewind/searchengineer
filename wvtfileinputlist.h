#ifndef WVTFILEINPUTLIST_H
#define WVTFILEINPUTLIST_H
#include <QVector>
class WVTDocumentInfo;
class WVTFileInputList
{
private:
    QVector<WVTDocumentInfo*> inputList;
    int numClasses;
public:
     QVector<WVTDocumentInfo*> fileList;
    //methods
    WVTFileInputList(int i);
    ~WVTFileInputList();
    void addEntry(WVTDocumentInfo* wvtdocumentinfo);
    int getNumClasses(){return numClasses;}
    void getEntries();

};

#endif // WVTFILEINPUTLIST_H
