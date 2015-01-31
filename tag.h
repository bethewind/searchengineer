#ifndef TAG_H
#define TAG_H
#include "region.h"
#include <QString>
#include <QVector>
using namespace std;
class Element;
class HtmlParser;
class Tag : public Region
{
private :
        string tagName;
        bool startTag;
    QVector<string> htmlAttributes;
    Element* element;
    //int htmlAttrNum;//the num of htmlAttributes
public:
        Tag(void);
        Tag(Page* page,int start,int end,const string& tagName,bool startTag);

        string getTagName (){return tagName;}
        bool isStartTag () { return startTag;}
        string getHTMLAttribute (const string& name);
        bool hasHTMLAttribute (const string& name);
        ~Tag(void);
        friend class Element;
        friend class HtmlParser;
        //commen useful tag names
            static const string A;
            static const string ABBREV;
            static const string ACRONYM ;
            static const string ADDRESS;
            static const string APPLET;
            static const string AREA;
            static const string B;
            static const string BASE;
            static const string BASEFONT;
            static const string BDO ;
            static const string BGSOUND ;
            static const string BIG;
            static const string BLINK;
            static const string BLOCKQUOTE ;
            static const string BODY ;
            static const string BR ;
            static const string CAPTION ;
            static const string CENTER ;
            static const string CITE ;
            static const string CODE ;
            static const string COL ;
            static const string COLGROUP ;
            static const string COMMENT ;
            static const string DD ;
            static const string DEL ;
            static const string DFN ;
            static const string DIR ;
            static const string DIV ;
            static const string DL ;
            static const string DT ;
            static const string EM ;
            static const string EMBED ;
            static const string FONT ;
            static const string FRAME ;
            static const string IFRAME ;
            static const string FRAMESET ;
            static const string FORM ;
            static const string H1 ;
            static const string H2 ;
            static const string H3 ;
            static const string H4 ;
            static const string H5 ;
            static const string H6 ;
            static const string HEAD ;
            static const string HR ;
            static const string HTML ;
            static const string I ;
            static const string IMG;
            static const string INPUT ;
            static const string ISINDEX ;
            static const string KBD ;
            static const string LI ;
            static const string LINK ;
            static const string LISTING ;
            static const string MAP ;
            static const string MARQUEE ;
            static const string MENU ;
            static const string META ;
            static const string NEXTID ;
            static const string NOBR ;
            static const string NOEMBED;
            static const string NOFRAMES ;
            static const string OBJECT ;
            static const string OL ;
            static const string OPTION ;
            static const string P ;
            static const string PARAM ;
            static const string PLAINTEXT ;
            static const string PRE ;
            static const string SAMP ;
            static const string SCRIPT ;
            static const string SELECT ;
            static const string SMALL ;
            static const string SPACER ;
            static const string STRIKE ;
            static const string STRONG ;
            static const string STYLE ;
            static const string SUB ;
            static const string SUP ;
            static const string SPAN;
            static const string TABLE ;
            static const string TD ;
            static const string TEXTAREA ;
            static const string TH ;
            static const string TITLE ;
            static const string TR ;
            static const string TT ;
            static const string U ;
            static const string UL ;
            static const string VAR ;
            static const string WBR;
            static const string XMP ;
            //
            static const string INFO_NODE[6];
            static const string LARGE_NODES[2];
            static const string TABLE_NODES[2];
            static const string HEADING_TAGS[7];
            static const string INVALID_TAGS[7];
            static const string SPACING_TAGS[2];
            static const string NOLINK[8];
};

#endif // TAG_H
