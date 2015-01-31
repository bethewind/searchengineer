#include "termbuffer.h"
#include "util.h"
#include "fieldinfos.h"
TermBuffer::TermBuffer():term(0)
{
}
//clone this instance
TermBuffer::TermBuffer(const TermBuffer& clone)
{
    this->field = clone.field;
    this->text = clone.text;
    //this->textLength = clone.textLength;
    this->term = clone.term == 0 ? 0 : new Term(clone.term->field,clone.term->text);
}
//
Term* TermBuffer::toTerm()
{
    if (field.isEmpty())                            // unset
      return 0;

    if (term == 0)
      term = new Term(field, text);

    return term;
  }
//
void TermBuffer::reset()
{
    this->field.clear();
   // this->textLength = 0;
    DELETE(this->term);
 }
//
void TermBuffer::set(TermBuffer *other)
{
    this->text = other->text;
    this->field = other->field;
    DELETE(this->term);
    this->term = other->term;
}
//
void TermBuffer::read(IndexInput *input, FieldInfos *fieldInfos)
{
    this->term = 0;
    int start = input->readVInt();
    int length = input->readVInt();
    int totalLength = start + length;
    this->text.resize(totalLength);
    input->readChars(this->text, start, length);
    this->field = fieldInfos->fieldName(input->readVInt());
}
