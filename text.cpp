#include "text.h"

Text::Text(void)
{
}
Text::Text(Page* page, int start, int end, std::string text)
:Region(page,start,end)
{

        this->text = text;
    }
Text::~Text(void)
{
}
