#include "formbutton.h"

FormButton::FormButton()
{
}
FormButton::FormButton (Tag* startTag, Tag* endTag, Form* form)
    :Link(startTag, endTag,0)
{

        this->form = form;
    }
