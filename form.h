#ifndef FORM_H
#define FORM_H
#include "link.h"
class Form : public Link
{
public:
    Form();
    Form (Tag* startTag, Tag* endTag, QUrl* base);
};

#endif // FORM_H
