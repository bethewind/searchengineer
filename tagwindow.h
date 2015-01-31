#ifndef TAGWINDOW_H
#define TAGWINDOW_H
#include <string>
using namespace std;
class Element;
class Page;
class TagWindow
{
private:
   Element* emlment;
      string text;
      string anchorText;
      int numInfoNodes;
      bool isHaveInput;
      double weightnum;
public:
    TagWindow();
    TagWindow(Element* e,const string& text,const string& anchorText,int numInfoNodes,int numInput);
    string getText(){return text;}
    bool isHInput(){return isHaveInput;}
    double fn(double x) {return x > 0.8f ? 0.8f : x;}
    double weight(Page* page);
    double anchorDensity();
    double share(int totalA, int totalT);
};

#endif // TAGWINDOW_H
