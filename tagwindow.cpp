#include "tagwindow.h"
#include "page.h"
#include "htmlparser.h"
TagWindow::TagWindow()
{
}
TagWindow::TagWindow(Element* e,const string& text,const string& anchorText,int numInfoNodes,int numInput)
{
    this->emlment = e;
    this->text = text;
    this->anchorText = anchorText;
    this->numInfoNodes = numInfoNodes;
    this->isHaveInput = numInput > 0 ? true : false;
}
double TagWindow::weight(Page *page)
{
    int totalT = page->text.length(),totalA = page->anchorText.length(),totalNumInfoNodes = page->numInfoNodes;
    double weight = 0;
         Element* e = this->emlment;
         weight += HtmlParser::isTableNodes(e) ? 0.1 : 0;
        weight += HtmlParser::isLargeNode(e) ? 0.1 : 0;
        weight += 0.2 * fn(numInfoNodes / (double) (totalNumInfoNodes));
        weight -= this->isHInput() ? 0.5 : 0;


    if (HtmlParser::containsNoise(text)) {
        weight -= 0.5;
    }

    weight += 1.0 - anchorDensity();
    weight += share(totalA, totalT);
    this->weightnum = weight;
    return weight;

}
// anchorLen/textLen
double TagWindow::anchorDensity()
{
        int anchorLen = anchorText.length();
        int textLen = text.length();
        if (anchorLen == 0 || textLen == 0) {
            return 0;
        }
        return anchorLen / (double) textLen;
 }
//
double TagWindow::share(int totalA, int totalT) {
        if (totalA == 0) {
            return 1.6 * fn((double) text.length() / totalT);
        }
        return 1.6 * fn((double) text.length() / totalT) - 0.8 * anchorText.length() / totalA;
    }
