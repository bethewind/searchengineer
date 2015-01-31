#include "element.h"
#include "tag.h"
#include "util.h"
Element::Element(void)
{
}
Element::Element (Tag* startTag, Tag* endTag)
:Region(startTag->source,startTag->start,endTag != 0 ? endTag->end : startTag->end)
  ,sibling(0),parent(0),child(0)
{

            this->startTag = startTag;
        this->endTag = endTag;
}
string Element::getTagName () {return startTag->getTagName();}
Element::~Element(void)
{
//    DELETE(startTag);
//    DELETE(endTag);
}
