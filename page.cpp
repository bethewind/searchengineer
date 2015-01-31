#include "page.h"
#include "htmlparser.h"
#include "text.h"
#include "tagwindow.h"
Page::Page(void):nWords(0),nTotal(0)
{
}
Page::Page(QUrl* url, const string& html,HtmlParser& parser)
:Region(0,0,html.length()),nWords(0),nTotal(0)
{
   source = this;
   base = url;
   this->content = html;
   parser.parse(this);
}
Page::Page(const string& content)
:Region(0,0,content.length()),nWords(0),nTotal(0)
{
   source = this;
   this->content = content;
}
//get the text int the tag
string Page::substringText (int start, int end) {
        if (words.isEmpty())
            return ""; // page is not parsed

        // FIX: find some other mapping
        string buf;
        QVector<Region*> rWords(nWords);
        for(int i = 0;i < nWords;i++)
        {
            rWords[i] = words[i];
        }
        for (int j = findStart (rWords, start,nWords); j<nWords; ++j) {
            if (words[j]->getEnd() > end)
                break;
            else {
                if (buf.length() > 0)
                    buf.append (1,' ');
                buf.append (words[j]->getText());
            }
        }
        return buf;
    }
string Page::substringContent (int start, int end)
{
     return getContent().substr(start, end-start);
}
Page::~Page(void)
{
    delete base;
    base = 0;
//        delete[] links;
//        links = 0;
        for(int i = 0;i < nTotal;i++)
        {

            if(tokens[i]!=0)
            {
               Region* r = tokens.at(i);
               delete r;
               r = 0;
            }
        }
        tokens.clear();
//        delete[] tokens;
//        tokens = 0;
//        delete[] words;
//        words = 0;
//        delete[] tags;
//        tags = 0;
        for(int i = 0;i < nElem;i++)
        {
            delete elements[i];
            elements[i] = 0;
        }
        for(int i = 0;i < tagWindows.size();i++)
        {
            delete tagWindows[i];
            tagWindows[i] = 0;
        }

}
