#include "term.h"

Term::Term()
{
}
Term::Term(QString fld, QString txt)
{
    this->field = fld;
    this->text = txt;
}
//
void Term::set(QString fld, QString txt)
{
    this->field = fld;
    this->text = txt;
}
//Compares two terms, returning a negative integer if this term belongs before
//the argument, zero if this term is equal to the argument, and a positive
//integer if this term belongs after the argument. The ordering of terms is
//first by field, then by text.
int Term::compareTo(Term* other)
{
    if (field == other->field)			  // fields are interned
      return text.compare(other->text);
    else
      return field.compare(other->field);
}
//
bool Term::operator ==(Term* other)
{
    return (this->compareTo(other)==0);
}
