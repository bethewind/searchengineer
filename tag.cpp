#include "tag.h"
const string Tag::A = "a";
const string Tag::ABBREV = "abbrev";
const string Tag::ACRONYM = "acronym";
const string Tag::ADDRESS = "address";
const string Tag::APPLET = "applet";
const string Tag::AREA = "area";
const string Tag::B = "b";
const string Tag::BASE = "base";
const string Tag::BASEFONT = "basefont";
const string Tag::BDO = "bdo";
const string Tag::BGSOUND = "bgsound";
const string Tag::BIG = "big";
const string Tag::BLINK = "blink";
const string Tag::BLOCKQUOTE = "blockquote";
const string Tag::BODY = "body";
const string Tag::BR = "br";
const string Tag::CAPTION = "caption";
const string Tag::CENTER = "center";
const string Tag::CITE = "cite";
const string Tag::CODE = "code";
const string Tag::COL = "col";
const string Tag::COLGROUP = "colgroup";
const string Tag::COMMENT = "!";
const string Tag::DD = "dd";
const string Tag::DEL = "del";
const string Tag::DFN = "dfn";
const string Tag::DIR = "dir";
const string Tag::DIV = "div";
const string Tag::DL = "dd";
const string Tag::DT = "dt";
const string Tag::EM = "em";
const string Tag::EMBED = "embed";
const string Tag::FONT = "font";
const string Tag::FRAME = "frame";
const string Tag::IFRAME = "iframe";
const string Tag::FRAMESET = "frameset";
const string Tag::FORM = "form";
const string Tag::H1 = "h1";
const string Tag::H2 = "h2";
const string Tag::H3 = "h3";
const string Tag::H4 = "h4";
const string Tag::H5 = "h5";
const string Tag::H6 = "h6";
const string Tag::HEAD = "head";
const string Tag::HR = "hr";
const string Tag::HTML = "html";
const string Tag::I = "i";
const string Tag::IMG = "img";
const string Tag::INPUT = "input";
const string Tag::ISINDEX = "isindex";
const string Tag::KBD = "kbd";
const string Tag::LI = "li";
const string Tag::LINK = "link";
const string Tag::LISTING = "listing";
const string Tag::MAP = "map";
const string Tag::MARQUEE = "marquee";
const string Tag::MENU = "menu";
const string Tag::META = "meta";
const string Tag::NEXTID = "nextid";
const string Tag::NOBR = "nobr";
const string Tag::NOEMBED = "noembed";
const string Tag::NOFRAMES = "noframes";
const string Tag::OBJECT = "object";
const string Tag::OL = "ol";
const string Tag::OPTION = "option";
const string Tag::P = "p";
const string Tag::PARAM = "param";
const string Tag::PLAINTEXT = "plaintext";
const string Tag::PRE = "pre";
const string Tag::SAMP = "samp";
const string Tag::SCRIPT = "script";
const string Tag::SELECT = "select";
const string Tag::SMALL = "small";
const string Tag::SPACER = "spacer";
const string Tag::STRIKE = "strike";
const string Tag::STRONG = "strong";
const string Tag::STYLE = "style";
const string Tag::SUB = "sub";
const string Tag::SUP = "sup";
const string Tag::SPAN = "span";
const string Tag::TABLE = "table";
const string Tag::TD = "td";
const string Tag::TEXTAREA = "textarea";
const string Tag::TH = "th";
const string Tag::TITLE = "title";
const string Tag::TR = "tr";
const string Tag::TT = "tt";
const string Tag::U = "u";
const string Tag::UL = "ul";
const string Tag::VAR = "var";
const string Tag::WBR = "wbr";
const string Tag::XMP = "xmp";
//
const string Tag::LARGE_NODES[2] = {Tag::DIV,Tag::TABLE};
    const string Tag::TABLE_NODES[2] = {Tag::TR, Tag::TD};
   const string Tag::INFO_NODE[6] ={Tag::P,Tag::SPAN,Tag::H1,Tag::H2,Tag::B,Tag::I};
    const string Tag::HEADING_TAGS[7] = {Tag::TITLE, Tag::H1, Tag::H2, Tag::H3, Tag::H4, Tag::H5, Tag::H6};
    const string Tag::INVALID_TAGS[7] = {Tag::STYLE, Tag::COMMENT, Tag::SCRIPT, Tag::OPTION,Tag::FRAME,Tag::FRAMESET,Tag::IFRAME};
   const string Tag::SPACING_TAGS[2] = {Tag::BR, Tag::SPAN};
   const string Tag::NOLINK[8] = {".css",".js",".gif",".jpeg",".bmp",".psd",".png",".jpg"};

Tag::Tag(void)
{
}
Tag::Tag(Page *page, int start, int end, const string &tagName, bool startTag)
:Region(page,start,end)
{
        this->tagName = QString(tagName.c_str()).toLower().toStdString();
        this->startTag = startTag;
        //this->htmlAttributes = 0;
}

string Tag::getHTMLAttribute (const string& name) {
        if (htmlAttributes.isEmpty())
            return "";
        string name1 = QString(name.c_str()).toLower().toStdString();
        for (int i=0; i<htmlAttributes.size(); ++i)
            if (htmlAttributes[i] == name1)
                return getLabel (name1);
        return "";
    }
bool Tag::hasHTMLAttribute (const string& name) {
        if (htmlAttributes.isEmpty())
            return false;
        string name1 = QString(name.c_str()).toLower().toStdString();
        for (int i=0; i<htmlAttributes.size(); ++i)
            if (htmlAttributes[i] == name1)
                return true;
        return false;
    }
Tag::~Tag(void)
{
//        delete[] htmlAttributes;
//        htmlAttributes = 0;
}
