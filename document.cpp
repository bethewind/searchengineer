#include "document.h"

Document::Document():boost(1.0)
{
}
//
void Document::add(Field *field)
{
    this->fields.append(field);
}

