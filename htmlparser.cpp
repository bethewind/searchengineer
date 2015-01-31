#include "htmlparser.h"
#include "page.h"
#include "text.h"
#include <QChar>
#include <QList>
#include<ctype.h>
#include "form.h"
#include "formbutton.h"
#include "tagwindow.h"
#include <QFile>
#include <QTextStream>
using std::vector;
typedef HtmlParser::Hashtable2<string,string>* hashptr;
bool HtmlParser::isinit = true;
HtmlParser::Hashtable2<string,string>* HtmlParser::headtag = (new Hashtable2<string,string>())
                                                             ->add(Tag::META)
                                                                       ->add(Tag::TITLE)
                                                                       ->add(Tag::BASE)
                                                                       ->add(Tag::LINK)
                                                                       ->add(Tag::ISINDEX)
                                                                       ;
HtmlParser::Hashtable2<string,string>* HtmlParser::savetext = (new Hashtable2<string,string>())
                                                              ->add(Tag::A)
                                                                          ->add(Tag::TITLE);
HtmlParser::Hashtable2<string,string>* HtmlParser::linktag = (new Hashtable2<string,string>())
                                                             ->add(Tag::A, "href")
                                                                         ->add(Tag::AREA, "href")
                                                                         ->add(Tag::APPLET, "code")
                                                                         ->add(Tag::EMBED, "src")
                                                                         ->add(Tag::FRAME, "src")
                                                                         ->add(Tag::FORM, "action")
                                                                         ->add(Tag::IMG, "src")
                                                                         ->add(Tag::LINK, "href")
                                                                         ->add(Tag::SCRIPT, "src")
                                                                         ;
HtmlParser::Hashtable2<string,hashptr>* HtmlParser::context = (new HtmlParser::Hashtable2<string,hashptr>())
                                                                                 ->add(Tag::DD, (new HtmlParser::Hashtable2<string,string>()) ->add(Tag::DL))
                                                                                          ->add(Tag::DT, (new HtmlParser::Hashtable2<string,string>()) ->add(Tag::DL))
                                                                                          ->add(Tag::LI, (new HtmlParser::Hashtable2<string,string>()) ->add(Tag::OL) ->add(Tag::UL) ->add(Tag::MENU) ->add(Tag::DIR))
                                                                                          ->add(Tag::OPTION, (new HtmlParser::Hashtable2<string,string>()) ->add(Tag::SELECT))
                                                                                          ->add(Tag::TR, (new HtmlParser::Hashtable2<string,string>()) ->add(Tag::TABLE))
                                                                                          ->add(Tag::TD, (new HtmlParser::Hashtable2<string,string>()) ->add(Tag::TR) ->add(Tag::TABLE))
                                                                                          ->add(Tag::TH, (new HtmlParser::Hashtable2<string,string>()) ->add(Tag::TR) ->add(Tag::TABLE))
                                                                                          ;
HtmlParser::Hashtable2<string,hashptr>* HtmlParser::forcesClosed = (new HtmlParser::Hashtable2<string,hashptr>())
                                                                                      ->add(Tag::DD, (new Hashtable2<string,string>()) ->add(Tag::DD) ->add(Tag::DT))
                                                                                                ->add(Tag::DT, (new HtmlParser::Hashtable2<string,string>()) ->add(Tag::DD) ->add(Tag::DT))
                                                                                                ->add(Tag::LI, (new HtmlParser::Hashtable2<string,string>()) ->add(Tag::LI))
                                                                                                ->add(Tag::OPTION, (new HtmlParser::Hashtable2<string,string>()) ->add(Tag::OPTION))
                                                                                                ->add(Tag::TR, (new HtmlParser::Hashtable2<string,string>()) ->add(Tag::TR))
                                                                                                ->add(Tag::TD, (new HtmlParser::Hashtable2<string,string>()) ->add(Tag::TD) ->add(Tag::TH))
                                                                                                ->add(Tag::TH,(new HtmlParser::Hashtable2<string,string>()) ->add(Tag::TD) ->add(Tag::TH))
                                                                                                ;
HtmlParser::Hashtable2<string,string>* HtmlParser::blocktag = (new Hashtable2<string,string>())
                                                              ->add(Tag::P)
                                                                        ->add(Tag::UL)
                                                                        ->add(Tag::OL)
                                                                        ->add(Tag::DIR)
                                                                        ->add(Tag::MENU)
                                                                        ->add(Tag::PRE)
                                                                        ->add(Tag::XMP)
                                                                        ->add(Tag::LISTING)
                                                                        ->add(Tag::DL)
                                                                        ->add(Tag::DIV)
                                                                        ->add(Tag::CENTER)
                                                                        ->add(Tag::BLOCKQUOTE)
                                                                        ->add(Tag::FORM)
                                                                        ->add(Tag::ISINDEX)
                                                                        ->add(Tag::HR)
                                                                        ->add(Tag::TABLE)
                                                                        ->add(Tag::H1)
                                                                        ->add(Tag::H2)
                                                                        ->add(Tag::H3)
                                                                        ->add(Tag::H4)
                                                                        ->add(Tag::H5)
                                                                        ->add(Tag::H6)
                                                                        ->add(Tag::ADDRESS)
                                                                        ;
HtmlParser::Hashtable2<string,string>* HtmlParser::empty = (new Hashtable2<string,string>()) ->add(Tag::AREA)
                                                           ->add(Tag::BASE)
                                                           ->add(Tag::BASEFONT)
                                                           ->add(Tag::BGSOUND)
                                                           ->add(Tag::BR)
                                                           ->add(Tag::COL)
                                                           ->add(Tag::COLGROUP)
                                                           ->add(Tag::COMMENT) // actually <!-- ... -->
                                                           ->add(Tag::HR)
                                                           ->add(Tag::IMG)
                                                           ->add(Tag::INPUT)
                                                           ->add(Tag::ISINDEX)
                                                           ->add(Tag::LINK)
                                                           ->add(Tag::META)
                                                           ->add(Tag::NEXTID)
                                                           ->add(Tag::PARAM)
                                                           ->add(Tag::SPACER)
                                                           ->add(Tag::WBR)
                                                           ;
 HtmlParser::Hashtable2<string,char>* HtmlParser::entities = (new Hashtable2<string,char>())->add ("quot",(char)34)
                                                                   ->add ("amp",(char)38)
                                                                             ->add ("lt",(char)60)
                                                                             ->add ("gt",(char)62)
                                                                             ->add ("nbsp",(char)160)
                                                                             ->add ("iexcl",(char)161)
                                                                             ->add ("cent",(char)162)
                                                                             ->add ("pound",(char)163)
                                                                             ->add ("curren",(char)164)
                                                                             ->add ("yen",(char)165)
                                                                             ->add ("brvbar",(char)167)
                                                                             ->add ("sect",(char)167)
                                                                             ->add ("uml",(char)168)
                                                                             ->add ("copy",(char)169)
                                                                             ->add ("ordf",(char)170)
                                                                             ->add ("laquo",(char)171)
                                                                             ->add ("not",(char)172)
                                                                             ->add ("shy",(char)173)
                                                                             ->add ("reg",(char)174)
                                                                             ->add ("macr",(char)175)
                                                                             ->add ("deg",(char)176)
                                                                             ->add ("plusmn",(char)177)
                                                                             ->add ("sup2",(char)178)
                                                                             ->add ("sup3",(char)179)
                                                                             ->add ("acute",(char)180)
                                                                             ->add ("micro",(char)181)
                                                                             ->add ("para",(char)182)
                                                                             ->add ("middot",(char)183)
                                                                             ->add ("cedil",(char)184)
                                                                             ->add ("sup1",(char)185)
                                                                             ->add ("ordm",(char)186)
                                                                             ->add ("raquo",(char)187)
                                                                             ->add ("frac14",(char)188)
                                                                             ->add ("frac12",(char)189)
                                                                             ->add ("frac34",(char)190)
                                                                             ->add ("iquest",(char)191)
                                                                             ->add ("Agrave",(char)192)
                                                                             ->add ("Aacute",(char)193)
                                                                             ->add ("Acirc",(char)194)
                                                                             ->add ("Atilde",(char)195)
                                                                             ->add ("Auml",(char)196)
                                                                             ->add ("Aring",(char)197)
                                                                             ->add ("AElig",(char)198)
                                                                             ->add ("Ccedil",(char)199)
                                                                             ->add ("Egrave",(char)200)
                                                                             ->add ("Eacute",(char)201)
                                                                             ->add ("Ecirc",(char)202)
                                                                             ->add ("Euml",(char)203)
                                                                             ->add ("Igrave",(char)204)
                                                                             ->add ("Iacute",(char)205)
                                                                             ->add ("Icirc",(char)206)
                                                                             ->add ("Iuml",(char)207)
                                                                             ->add ("ETH",(char)208)
                                                                             ->add ("Ntilde",(char)209)
                                                                             ->add ("Ograve",(char)210)
                                                                             ->add ("Oacute",(char)211)
                                                                             ->add ("Ocirc",(char)212)
                                                                             ->add ("Otilde",(char)213)
                                                                             ->add ("Ouml",(char)214)
                                                                             ->add ("times",(char)215)
                                                                             ->add ("Oslash",(char)216)
                                                                             ->add ("Ugrave",(char)217)
                                                                             ->add ("Uacute",(char)218)
                                                                             ->add ("Ucirc",(char)219)
                                                                             ->add ("Uuml",(char)220)
                                                                             ->add ("Yacute",(char)221)
                                                                             ->add ("THORN",(char)222)
                                                                             ->add ("szlig",(char)223)
                                                                             ->add ("agrave",(char)224)
                                                                             ->add ("aacute",(char)225)
                                                                             ->add ("acirc",(char)226)
                                                                             ->add ("atilde",(char)227)
                                                                             ->add ("auml",(char)228)
                                                                             ->add ("aring",(char)229)
                                                                             ->add ("aelig",(char)230)
                                                                             ->add ("ccedil",(char)231)
                                                                             ->add ("egrave",(char)232)
                                                                             ->add ("eacute",(char)233)
                                                                             ->add ("ecirc",(char)234)
                                                                             ->add ("euml",(char)235)
                                                                             ->add ("igrave",(char)236)
                                                                             ->add ("iacute",(char)237)
                                                                             ->add ("icirc",(char)238)
                                                                             ->add ("iuml",(char)239)
                                                                             ->add ("eth",(char)240)
                                                                             ->add ("ntilde",(char)241)
                                                                             ->add ("ograve",(char)242)
                                                                             ->add ("oacute",(char)243)
                                                                             ->add ("ocirc",(char)244)
                                                                             ->add ("otilde",(char)245)
                                                                             ->add ("ouml",(char)246)
                                                                             ->add ("divide",(char)247)
                                                                             ->add ("oslash",(char)248)
                                                                             ->add ("ugrave",(char)249)
                                                                             ->add ("uacute",(char)250)
                                                                             ->add ("ucirc",(char)251)
                                                                             ->add ("uuml",(char)252)
                                                                             ->add ("yacute",(char)253)
                                                                             ->add ("thorn",(char)254)
                                                                             ->add ("yuml",(char)255)
                                                                   ;
HtmlParser::HtmlParser(void)
    :openPtr(0),openElems(new int[20]),openELen(20)
{
    if(isinit)
    {
        Hashtable2<string,string>* p = (new Hashtable2<string,string>())->add(Tag::P);
        const QList<string>& klist = blocktag->map1.keys();
        for(int i = 0;i < klist.length();i++)
        {
            union1(forcesClosed,klist.at(i),p);
        }
        const QList<string>& fklist = forcesClosed->map1.keys();
        for(int i = 0;i < fklist.length();i++)
        {
            string tagname = fklist.at(i);
            union1(context,tagname,forcesClosed->map1.value(tagname));
        }
        isinit = false;
    }
}
void HtmlParser::parse(Page* page)
{
    tokenize(page);
    buildParseTree(page);
    textExtraction(page);
}
//purifier the page and extrater the text
void HtmlParser::textExtraction(Page* page)
{
//        QFile file("data/wei11.txt");
//        if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
//            return;
//        QTextStream out(&file);

    for(int i = 0;i < page->elements.size();i++)
    {
        if(page->elements[i]->getTagName() == Tag::BODY)
        {
            Element* e = page->elements[i];
            HtmlText htmlText = getHtmlText(e,page);
            page->text = htmlText.text;
            //out << QObject::tr(page->text.c_str()) << "\n\n";
            page->anchorText = htmlText.anchorText;
           // out << "-----------------Links--------------\n";
            //out << QObject::tr(page->anchorText.c_str());
            page->numInfoNodes = htmlText.numInfoNodes;
            break;
        }
    }
    if(page->tagWindows.size() == 0)
        return;
    TagWindow* tagWindow =page->tagWindows.at(0);
    double largestWeight = page->tagWindows.at(0)->weight(page);



    //get the tagWindow which have the largest weight
    for(int i = 1;i < page->tagWindows.size();i++)
    {
//        double weight11 = page->tagWindows.at(i)->weight(page);
//        QString content11 = QObject::tr(page->tagWindows.at(i)->getText().c_str());
//        out << "///////////////////////document///////////////////////\n";
//        out << weight11 << "\n";
//        out << content11 << "\n";
        if(largestWeight < page->tagWindows.at(i)->weight(page))
        {
            largestWeight = page->tagWindows.at(i)->weight(page);
            tagWindow =page->tagWindows.at(i);
        }
    }
//    out << "///////////////////////document///////////////////////\n";
//    out << largestWeight << "\n";
//    out << QObject::tr(tagWindow->getText().c_str()) << "\n";

//    for(int i = 0;i < page->getElem();i++)
//          {
//              QString aaaaa = QObject::tr(page->elements[i]->toText().c_str());
//              QString tagname = page->elements[i]->getStartTag()->getTagName().c_str();
//
//             out << "[" << page->elements[i]->getStart() << "," << page->elements[i]->getEnd() << tagname<< "]"  << aaaaa << "\n";
//          }
    //out << QObject::tr(page->toString().c_str()) << "\n";
     //out << QObject::tr(page->toText().c_str()) << "\n";

//    file.flush();
//    file.close();
    page->importantText = tagWindow->getText();
}
bool HtmlParser::isTableNodes(Element* e)
{
    for(unsigned int i = 0;i < sizeof(Tag::TABLE_NODES)/sizeof(Tag::TABLE_NODES[0]);i++)
    {
        if(e->getTagName()==Tag::TABLE_NODES[i])
        {
            return true;
        }
    }

    return false;
}
//
bool HtmlParser::isNoLinks(string e)
{
    for(unsigned int i = 0;i < sizeof(Tag::NOLINK)/sizeof(Tag::NOLINK[0]);i++)
    {
        if(e==Tag::NOLINK[i])
        {
            return true;
        }
    }

    return false;
}
bool HtmlParser::isLargeNode(Element* e)
{
    for(unsigned int i = 0;i < sizeof(Tag::LARGE_NODES)/sizeof(Tag::LARGE_NODES[0]);i++)
    {
        if(e->getTagName()==Tag::LARGE_NODES[i])
        {
            return true;
        }
    }
    return false;
}
bool HtmlParser::containsNoise(const string& text)
{
    string txt = QString(text.c_str()).toLower().toStdString();
    if(txt.find("copyright") != std::string::npos
       ||txt.find("all rights reserved") != std::string::npos
       ||txt.find("版权所有") != std::string::npos
       ||txt.find("上一页") != std::string::npos
       ||txt.find("下一页") != std::string::npos
       ||txt.find("ICP备") != std::string::npos
       )
        return true;
    return false;
}
//judge the element is or not a infonode
bool HtmlParser::isInfoNode(Element* e)
{
    for(unsigned int i = 0;i < sizeof(Tag::INFO_NODE)/sizeof(Tag::INFO_NODE[0]);i++)
    {
        if(e->getTagName()==Tag::INFO_NODE[i])
        {
            return true;
        }
    }
    return false;
}
//
bool HtmlParser::isInvalidElement(Element* e)
{
    for(unsigned int i = 0;i < sizeof(Tag::INVALID_TAGS)/sizeof(Tag::INVALID_TAGS[0]);i++)
    {
        if(e->getTagName()==Tag::INVALID_TAGS[i])
        {
            return true;
        }
    }
    return false;
}
//get text,anchortext,numInfoNodes of the element
HtmlText HtmlParser::getHtmlText(Element* element,Page* page)
{
//    if(element->getStart() == 34867)
//        int aaaaa = 1;
    HtmlText ht = {"","",0,0};
     if(isInvalidElement(element))
    {
         return ht;
    }
      if(element->getTagName() == Tag::INPUT)
     {

         if(element->getStartTag()->getLabel(Tag::INPUT) != "hidden")
          {
             ht.numInput += 1;
         }
     }
    if(isInfoNode(element))
    {
        ht.numInfoNodes +=1;
    }
    Element* c = element->getChild();
    if(c == 0)
    {

        ht.text += element->toText();


    }
    else
    {
        if(element->getTagName() == Tag::A)
        {
           ht.anchorText += element->toText();
        }

        for(Element* e = c;e != 0;e = e->getSibling())
        {
            HtmlText ht1 = getHtmlText(e,page);
            ht.text += ht1.text;
            ht.anchorText += ht1.anchorText;
            ht.numInfoNodes += ht1.numInfoNodes;
            ht.numInput += ht1.numInput;

        }
    }
    //ht.text = element->toText();
    if(ht.text !="")
    {
        TagWindow* tagWindow = new TagWindow(element,ht.text,ht.anchorText,ht.numInfoNodes,ht.numInput);
        page->tagWindows.append(tagWindow);
    }
     return ht;
}
//parse the tag and text tokens in the page
void HtmlParser::tokenize (Page* page)
{
        int state = START;
        string content = page->getContent();
        int buflen = content.length();
        int bufptr = 0;//point where is parsing
        int bufbase = 0;

        //token list
        //QVector<Region*> tokens;
        int wordStart = 0;
        page->nWords = 0;  //a total number of text tokens
        Tag* tag = 0;
        int tagStart = 0;
        int entnum = 0;
     string entityTargetBuf = "";
          int postEntityState = 0;
         while(bufptr < buflen)
         {



            char c = content.at(bufptr);

            //System.err.println ("%% state == " + state + ", ptr == " + (bufbase+bufptr) + ", c == " + c);

            switch (state) {
                case START:
                    // after whitespace or tag
                    switch (c) {
                        case '<':
                            ++bufptr;
                            state = LT;
                            break;
                        case ' ':
                        case '\t':
                        case '\n':
                        case '\r':
                            ++bufptr;
                            break;
                        default:
                                                        wordBuf.clear();
                            wordStart = bufbase+bufptr;
                            state = INWORD;
                            break;
                    }
                    break;

                case INWORD:
                    // Character data
                    switch (c) {
                        case '<':
                            page->tokens.push_back(new Text (page, wordStart, bufbase+bufptr, wordBuf));
                            ++(page->nWords);
                            //add a virtual tag
                            tag = new Tag (page, wordStart-1, wordStart,"myText", true);
                            page->tokens.push_back(tag);
                            tag = new Tag (page, bufbase+bufptr, bufbase+bufptr,"myText", false);
                            page->tokens.push_back(tag);

                            state = START;
                            break;
                        case ' ':
                        case '\t':
                        case '\n':
                        case '\r':
                             page->tokens.push_back(new Text (page, wordStart, bufbase+bufptr, wordBuf));
                            ++(page->nWords);
                            //add a virtual tag
                            tag = new Tag (page, wordStart-1, wordStart,"myText", true);
                            page->tokens.push_back(tag);
                            tag = new Tag (page, bufbase+bufptr, bufbase+bufptr,"myText", false);
                            page->tokens.push_back(tag);

                            state = START;
                            ++bufptr;
                            break;
                        case '&':
                            ++bufptr;
                            postEntityState = INWORD;
                            entityTargetBuf = wordBuf;
                            state = ENTITY;
                            break;
                        default:
                            wordBuf.append (1,(char)c);
                            ++bufptr;
                            // state == INWORD;
                            break;
                    }
                    break;

                //  Entities
                case ENTITY:
                    if (c == '#') {
                        ++bufptr;
                        entnum = 0;
                        state = ENTNUM;
                    }
                    else if ((c >= 'A' && c <= 'Z')
                             || (c >= 'a' && c <= 'z')) {
                                                                 entity.clear();
                        state = ENTREF;
                    }
                    else {
                        entityTargetBuf.append (1,'&');
                        state = postEntityState;
                    }
                    break;

                case ENTREF:
                    if (!QChar(c).isLetterOrNumber()) {

                        if (entities->map1.contains(entity)) {
                            entityTargetBuf.append (1,entities->map1.value(entity));
                            if (c == ';')
                                ++bufptr;
                        }
                        else {
                            // unrecognized entity -- leave
                            // as-is
                            entityTargetBuf.append (1,'&');
                            entityTargetBuf.append (entity);
                        }
                        state = postEntityState;
                    }
                    else {
                        ++bufptr;
                        entity.append (1,c);
                        // state == ENTREF;
                    }
                    break;

                case ENTNUM:
                    if (c==';' || !QChar(c).isDigit()) {
                        entityTargetBuf.append (1,(char) entnum);
                        if (c == ';')
                            ++bufptr;
                        state = postEntityState;
                    }
                    else {
                        entnum = 10*entnum + (c - '0');
                        ++bufptr;
                    }
                    break;

                case LT:
                    tagStart = bufbase+bufptr-1;
                    switch (c) {
                        case '/':
                            ++bufptr;
                            tagName.clear();;
                            state = ETAG;
                            break;
                        case '!':
                            ++bufptr;
                            state = BANG;
                            break;
                        default:
                            if (QChar(c).isLetter()) {
                                tagName.clear();
                                state = STAG;
                            }
                            else {
                                wordBuf.append (1,'<');
                                state = INWORD;
                            }
                            break;
                    }
                    break;

                // Comments and directives.
                // Implements the (broken, but easy) Netscape rule:
                // <!-- starts a comment, --> closes.
                // All other directives <!foo> are also returned as comments.
                case BANG:
                    if (c == '-') {
                        ++bufptr;
                        state = BANG_DASH;
                    }
                    else {
                        state = DIRECTIVE;
                    }
                    break;

                case BANG_DASH:
                    if (c == '-') {
                        ++bufptr;
                        state = CMT;
                    }
                    else {
                        state = DIRECTIVE;
                    }
                    break;

                case CMT:
                    if (c == '-') {
                        ++bufptr;
                        state = CMT_DASH;
                    }
                    else {
                        ++bufptr;
                    }
                    break;

                case CMT_DASH:
                    if (c == '-') {
                        ++bufptr;
                        state = CMT_DASHDASH;
                    }
                    else {
                        ++bufptr;
                        state = CMT;
                    }
                    break;

                case CMT_DASHDASH:
                    if (c == '>') {
                        ++bufptr;
                        tag = new Tag (page, tagStart, bufbase+bufptr, Tag::COMMENT, true);
                        page->tokens.push_back(tag);
                        state = START;
                    }
                    else if (c == '-') {
                        ++bufptr;
                        state = CMT_DASHDASH;
                    }
                    else {
                        ++bufptr;
                        state = CMT;
                    }
                    break;

                case DIRECTIVE:
                    if (c == '>') {
                        ++bufptr;
                        tag = new Tag (page, tagStart, bufbase+bufptr, Tag::COMMENT, true);
                        page->tokens.push_back (tag);
                        state = START;
                    }
                    else {
                        ++bufptr;
                    }
                    break;

                // Tags
                case STAG:
                    if (c == '>' || isspace(c)) {
                        tag = new Tag (page, tagStart, bufbase+bufptr, // tag doesn't really end here
                                                                       // -- we'll fix it up when we actually see it
                                       tagName, true);
                        page->tokens.push_back(tag);
                        tag->htmlAttributes.clear();;
                        state = ATTR;
                        //isHTML = true;
                    }
                    else {
                        tagName.append (1,c);
                        ++bufptr;
                        // state == STAG;
                    }
                    break;

                case ETAG:
                    if (c == '>') {
                        ++bufptr;
                        tag = new Tag (page, tagStart, bufbase+bufptr, tagName, false);
                        page->tokens.push_back (tag);
                        state = START;
                    }
                    else {
                        tagName.append (1,c);
                        ++bufptr;
                        // state == ETAG
                    }
                    break;

                // Attributes
                case ATTR:
                    if (isspace(c))
                        ++bufptr;
                    else if (c == '>') {
                        ++bufptr;
                        tag->end = bufbase+bufptr;

                        state = START;
                    }
                    else {
                        attrName.clear();;
                        state = ATTRNAME;
                    }
                    break;

                case ATTRNAME:
                    if (c == '>' || c == '=' || isspace(c)) {
                        state = EQ;
                    }
                    else {
                        attrName.append (1,c);
                        ++bufptr;
                        // state == ATTRNAME;
                    }
                    break;

                case EQ:
                    if (isspace(c))
                        ++bufptr;
                    else if (c == '=') {
                        ++bufptr;
                        state = AFTEREQ;
                    }
                    else {
                        string name = QString(attrName.c_str()).toLower().toStdString();
                        tag->setLabel(name);
                        tag->htmlAttributes.push_back(name);
                        state = ATTR;
                    }
                    break;

                case AFTEREQ:
                    if (isspace (c))
                        ++bufptr;
                    else
                        switch (c) {
                            case '>':
                            {
                            string name = QString(attrName.c_str()).toLower().toStdString();
                            tag->setLabel(name);
                            tag->htmlAttributes.push_back(name);
                                state = ATTR;
                                break;
                            }
                            case '\'':
                                ++bufptr;
                                attrVal.clear();
                                state = ATTRVAL_SQ;
                                break;
                            case '"':
                                ++bufptr;
                                attrVal.clear();
                                state = ATTRVAL_DQ;
                                break;
                            default:
                                attrVal.clear();
                                state = ATTRVAL;
                                break;
                        }
                    break;

                case ATTRVAL:
                    if (c == '>' || isspace(c)) {
                        string name = QString(attrName.c_str()).toLower().toStdString();
                        tag->setLabel(name,attrVal);
                        tag->htmlAttributes.push_back(name);

                        state = ATTR;
                    }
                    else if (c == '&') {
                        ++bufptr;
                        postEntityState = ATTRVAL;
                        entityTargetBuf = attrVal;
                        state = ENTITY;
                    }
                    else {
                        ++bufptr;
                        attrVal.append (1,c);
                        // state == ATTRVAL;
                    }
                    break;

                case ATTRVAL_SQ:
                    if (c=='\'') {
                        ++bufptr;
                        string name = QString(attrName.c_str()).toLower().toStdString();
                        tag->setLabel(name,attrVal);
                        tag->htmlAttributes.push_back(name);
                        state = ATTR;
                    }
                    else if (c == '&') {
                        ++bufptr;
                        postEntityState = ATTRVAL_SQ;
                        entityTargetBuf = attrVal;
                        state = ENTITY;
                    }
                    else {
                        ++bufptr;
                        attrVal.append (1,c);
                        // state == ATTRVAL_SQ;
                    }
                    break;

                case ATTRVAL_DQ:
                    if (c=='"') {
                        ++bufptr;
                        string name = QString(attrName.c_str()).toLower().toStdString();
                        tag->setLabel(name,attrVal);
                        tag->htmlAttributes.push_back(name);
                        state = ATTR;
                    }
                    else if (c == '&') {
                        ++bufptr;
                        postEntityState = ATTRVAL_DQ;
                        entityTargetBuf = attrVal;
                        state = ENTITY;
                    }
                    else {
                        ++bufptr;
                        attrVal.append (1,c);
                        // state == ATTRVAL_DQ;
                    }
                    break;

                default:
                    //throw new RuntimeException ("HtmlTokenizer entered illegal state " + state);
                    break;
            }

         }
         // EOF
                switch (state) {
                    case INWORD:
                        // EOF terminated some text -- save the text
                        page->tokens.push_back(new Text (page, wordStart, bufbase+bufptr, wordBuf));
                        ++(page->nWords);
                        break;

                    default:
                        // EOF in the middle of tags is illegal
                        // don't try to recover
                        break;
                }

                 page->nTotal = page->tokens.size ();

//                page->tokens = new Region*[page->nTotal];
//                page->tokens = tokens.data();

                //page->words = new Text*[page->nWords];
                //int textnum = 0;
                //page->tags = new Tag*[page->nTotal - page->nWords];
                //int tagnum = 0;

                for (int i=0; i < page->nTotal; ++i) {
                    if (Tag* tag11 = dynamic_cast<Tag*>(page->tokens[i]))
                        page->tags.append(tag11);
                    else{
                        if(Text* txt = dynamic_cast<Text*>(page->tokens[i]))
                        page->words.append(txt);
                    }
                }
     }
void HtmlParser::union1(Hashtable2<string,Hashtable2<string,string>* >* map,const string& tagname,Hashtable2<string,string>* tagset)
{
    if(map->map1.contains(tagname))
    {
        Hashtable2<string,string>* mmp1v = map->map1.value(tagname);
        map->add(tagname,mmp1v->union1(tagset));
    }
    else
    {
        map->add(tagname,tagset);
    }
}
void HtmlParser::buildParseTree (Page* page)
{

    bool keepText = false;

    elems.resize(0);
    openPtr = 0;

    //Region** tokens = page->tokens;
    for (int t=0; t < page->nTotal; ++t) {
       Region* r = page->tokens[t];

        if (Tag* tag = dynamic_cast<Tag*>(r)) {

            string tagName = tag->getTagName();

            if (tag->isStartTag()) {
                // start tag <X>

                // check if <X> forces closure of an open element
                if (forcesClosed->map1.contains(tagName)) {
                    Element* e = findOpenElement (*(context->map1.value(tagName)));
                    // Hashtable2<string,string>* fmv = forcesClosed->map1.value(tagName);
                    if (e != 0 && (forcesClosed->map1.value(tagName))->map1.contains((e->getTagName())))
                        close (e, tag->start);
                }

                // create the element and push it on the elems stack
                Element* e = makeElement (page->base, tag,page);
                open (e);

                if (empty->map1.contains(tagName)) {
                    // element has no content
                    // close it off right now
                    close (e, tag->end);
                }
                else if (savetext->map1.contains(tagName)) {
                    text.clear();
                    keepText = true;
                }

                if (tagName == Tag::BASE) {
                    string href = tag->getHTMLAttribute (string("href"));
                    if (href != "") {

                            QUrl* base1 = page->base;

                            page->base = new QUrl(Link::resolved(page->base,href));
                            delete base1;
                            base1 = 0;

                    }
                }
            } //end if (tag.isStartTag())
            else {
                // end tag </X>

                // find matching start tag <X>
                Element* e = findOpenElement (tagName);
                if (e != 0) {
                    close (e, tag);

                    if (savetext->map1.contains(tagName)) {
                        if (tagName == Tag::TITLE)
                            page->title = text;
                        else
                            if (Link* link = dynamic_cast<Link*>(e))
                            {

                            link->setText(QObject::tr(text.c_str()));
                            }
                        keepText = false;
                    }
                }
            }

        } //end if(r tag)
        else { // r is a text token
            if (keepText) {
                if (text.length() > 0)
                    text.append (1,' ');
                text.append (r->toText());
            }
        }
    } //end for

    // close any remaining open elements
    closeAll (page->end);

    // link together the top-level elements
    if (!elems.empty()) {
        int nElems = elems.size ();
        Element* c = elems.at(0);
        page->root = c;
        for (int j=1; j<nElems; ++j) {
            Element* d = elems.at(j);
            c->sibling = d;
            c = d;
        }
    }

//    page->elements = new Element*[vElements.size()];
//    page->elements = vElements.data();
    page->nElem =  page->elements.size();


//    page->links = new Link*[vLinks.size()];
//     page->links = vLinks.data();


}
void HtmlParser::closeAll (int end) {
        if (openPtr > 0)
            close (elems.at(openElems[0]), end);
    }
Element* HtmlParser::makeElement (QUrl* base, Tag* tag,Page* page) {
        Element* e = 0;
        string tagName = tag->getTagName();
        string hrefAttr = "";
        if(HtmlParser::linktag->map1.contains(tagName))
        {
            hrefAttr = HtmlParser::linktag->map1.value(tagName);
        }
        string type;


            if (tagName == Tag::FORM) {
               Link* l = new Form (tag, 0, base);
               e = l;
                  page->links.push_back(l);
            }
            else if (tagName == Tag::INPUT
                     && (type = tag->getHTMLAttribute (string("type"))) != ""
                     && (QString(type.c_str()).toLower().toStdString()=="submit" || QString(type.c_str()).toLower().toStdString()==string("image"))) {
                Link* l = new FormButton (tag, 0, currentForm);
                e = l;
                   page->links.push_back(l);
            }
            else if (hrefAttr != "" && tag->hasHTMLAttribute(hrefAttr)) {
                Link* l = new Link (tag, 0, base);
                e = l;
                   page->links.push_back(l);
            }


        if (e == 0)
            // just make an ordinary element
            e = new Element (tag, 0);

        page->elements.push_back(e);
        tag->element = e;
        return e;
    }
Element* HtmlParser::findOpenElement (const HtmlParser::Hashtable2<string,string>& tags)
{
        for (int i=openPtr-1; i >= 0; --i) {
            Element* e = elems.at(openElems[i]);
            if (tags.map1.contains(e->getTagName ()))
                return e;
        }
        return 0;
    }
Element* HtmlParser::findOpenElement (const string& tagname) {
        for (int i=openPtr-1; i >= 0; --i) {
            Element* e = elems.at(openElems[i]);
            if (tagname == e->getTagName ())
                return e;
        }
        return 0;
    }
void HtmlParser::close (Element* elem, int end) {
        int v;
        Element* e;
        do {
            v = openElems[--openPtr];
            e = elems.at(v);

            e->end = end;
            if (dynamic_cast<Form*>(e) !=0)
                currentForm = 0;

            int firstChild = v+1;
            int nElems = elems.size();
            if (firstChild < nElems) {
                Element* c = elems.at(firstChild);
                e->child = c;
                for (int j=firstChild+1; j<nElems; ++j) {
                    Element* d = elems.at(j);
                    c->sibling = d;
                    c = d;
                }
                elems.resize(firstChild);
            }

        } while (e != elem);
    }
void HtmlParser::close (Element* elem, Tag* tag) {
        elem->endTag = tag;
        tag->element = elem;
        close (elem, tag->start);
        elem->end = tag->end;
    }
void HtmlParser::open (Element* e) {
        if (openPtr > 0)
            e->parent = elems.at(openElems[openPtr-1]);
        else
            e->parent = 0;

        elems.push (e);
        if (Form* form = dynamic_cast<Form*>(e))
            currentForm = form;

        if (openPtr == openELen) {
            int *newarr  = new int[openELen + 10];

            for(int i = 0;i < openELen;i++)
            {
                newarr[i] = openElems[i];
            }
            delete []openElems;
            openElems = 0;
            openElems = newarr;
            openELen += 10;
        }
        openElems[openPtr] = elems.size()-1;
        ++openPtr;
    }
HtmlParser::~HtmlParser(void)
{
    delete []openElems;
    openElems = 0;
}
