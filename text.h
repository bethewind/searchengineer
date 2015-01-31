#ifndef TEXT_H
#define TEXT_H
#include "region.h"
#include <string>
using namespace std;
class Text : public Region
{
private:
    string text;
public:
        Text(void);
        Text (Page* page, int start, int end, string text);
        string getText(){return text;}
        ~Text(void);
};

#endif // TEXT_H
