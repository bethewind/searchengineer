#ifndef HTMLPARSER_H
#define HTMLPARSER_H
#include <QVector>
#include <QMap>
#include <QStack>
#include "tag.h"
//#include <memory>
using namespace std;
struct HtmlText
{
    string text;
    string anchorText;
    int numInfoNodes;
    int numInput;
};
class QUrl;
class Element;
class Link;
class Page;
class Form;
class HtmlParser
{
    friend class Link;
public:
         template<class Key,class V> class Hashtable2
        {
        public:
                //typedef QMap<Key,V> map;
                QMap<Key,V> map1;
                Hashtable2(){}
                Hashtable2* add(Key key)
                {
                       map1.insert(key,key);
                    return this;
                }
                Hashtable2* add(Key key,V val)
                {
                       map1.insert(key,val);
                    return this;
                }
                Hashtable2* union1(Hashtable2<Key,V>* map2)
                {
//                    QMap<Key,V>::const_iterator i = map2->constBegin();
//                        while (i != map2->constEnd())
//                        {
//                           map1.insert(i.key(),i.value());
//                            ++i;
//                        }
                        QMapIterator<Key,V> i(map2->map1);
                         while (i.hasNext()) {
                             i.next();
                             map1.insert(i.key(),i.value());
                         }


                        return this;
                }
        }; //class hashtable2
public:
         string wordBuf;
    string tagName;
    string attrName;
    string attrVal;
    QVector<string> attrs;
    string entity ;
//    QVector<Element*> vElements;
//        QVector<Link*> vLinks;
    static bool isInfoNode(Element* e);
    static bool isInvalidElement(Element* e);
     static bool isTableNodes(Element* e);
      static bool isLargeNode(Element* e);
      static bool isNoLinks(string e);

        static bool containsNoise(const string& text);

        HtmlParser(void);


        ~HtmlParser(void);
        void parse(Page* page);
   static Hashtable2<string,Hashtable2<string,string>* >* forcesClosed;
private:
        static const int START = 0;
    static const int INWORD = 1;
    static const int ENTITY = 2;
    static const int LT = 4;
    static const int BANG = 5;
    static const int BANG_DASH = 6;
    static const int CMT = 7;
    static const int CMT_DASH = 8;
    static const int CMT_DASHDASH = 9;
    static const int DIRECTIVE = 10;
    static const int STAG = 11;
    static const int ETAG = 12;
    static const int ATTR = 13;
    static const int ATTRNAME = 14;
    static const int EQ = 15;
    static const int AFTEREQ = 16;
    static const int ATTRVAL = 17;
    static const int ATTRVAL_SQ = 18;
    static const int ATTRVAL_DQ = 19;
    static const int DONE = 20;
    static const int ENTNUM = 21;
    static const int ENTREF = 22;
        //map
        static Hashtable2<string,char>* entities;
        static Hashtable2<string,string>* empty;
        static Hashtable2<string,string>* blocktag;

        static Hashtable2<string,Hashtable2<string,string>* >* context;
        //static Hashtable2<string,char>* literal1;
    static Hashtable2<string,string>* linktag;
        static Hashtable2<string,string>* savetext;
        static Hashtable2<string,string>* headtag;
        //methods

        void textExtraction(Page* page);
        HtmlText getHtmlText(Element* element,Page* page);
        void tokenize (Page* page);
        static void union1(Hashtable2<string,Hashtable2<string,string>* >* map,const string& tagname,Hashtable2<string,string>* tagset);
        void buildParseTree (Page* page);
        Element* makeElement (QUrl* base, Tag* tag,Page* page);
        Element* findOpenElement (const string& tagname);
        Element* findOpenElement (const Hashtable2<string,string>& tags);
         void open (Element* e);
          void close (Element* elem, int end);
          void close (Element* elem, Tag* tag) ;
          void closeAll (int end);
        static bool isinit;
        QStack<Element*> elems;
        int openPtr;

        int* openElems;
        int openELen;
         Form* currentForm;
         string text;


};

#endif // HTMLPARSER_H
