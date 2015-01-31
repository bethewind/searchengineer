#ifndef ELEMENT_H
#define ELEMENT_H
#include "region.h"
class Tag;
class HtmlParser;
class Element : public Region
{
    friend class HtmlParser;
protected:
        Tag* startTag;
        Tag* endTag;
        Element* sibling; // next sibling
        Element* parent;
        Element* child;   // first child
public:
        Element(void);
        string getTagName ();
        Tag* getStartTag (){return startTag;}
        Element (Tag* startTag, Tag* endTag);
        Element* getSibling () {return sibling;}
        Element* getChild () {return child;}
        virtual ~Element(void);
};

#endif // ELEMENT_H
