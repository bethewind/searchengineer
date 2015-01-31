#ifndef FORMBUTTON_H
#define FORMBUTTON_H
#include "link.h"
class Form;
class FormButton : public Link
{
private:
    Form* form;
public:
    FormButton();
    FormButton (Tag* startTag, Tag* endTag, Form* form);
};

#endif // FORMBUTTON_H
