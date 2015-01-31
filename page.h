#ifndef PAGE_H
#define PAGE_H
#include "region.h"
#include "link.h"
#include <QVector>
using std::string;
class HtmlParser;
class Text;
class TagWindow;
class Page : public Region
{
    friend class HtmlParser;
    friend class TagWindow;
private:
        Link origin;
        string title;
        QUrl* base;

        string content;

        QVector<Text*> words;
        QVector<Tag*> tags;
        QVector<Element*> elements;
         QVector<TagWindow*> tagWindows;
        Element* root;
        //
        int nWords;
        int nTotal;
        int nElem;
        string text;//all text in the page except stype/script
        string anchorText;//all text of the page int the a tag
        int numInfoNodes;
        string importantText;

public:
        QVector<Region*> tokens;
         QVector<Link*> links;

        Page(void);
        Page(QUrl* url,const string& html,HtmlParser& parser);
        Page(const string& content);
        string getImportantText(){return importantText;}
        string getContent(){return content;}
        string getTitle(){return title;}
        int getTotal(){return nTotal;}
         int getElem(){return nElem;}
        string substringText (int start, int end);
        string substringContent (int start, int end);
        ~Page(void);
};

#endif // PAGE_H
