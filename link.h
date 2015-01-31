#ifndef LINK_H
#define LINK_H
#include <QUrl>
#include "element.h"
#include <QString>
using std::string;

class Link : public Element
{
private:
        QUrl url;
        int depth;
        QString text;
public:
        Link(void);
        Link(const QString& href,int depth,int id);
        Link(const QString& href);
        Link(const QUrl& url);
        Link (Tag* startTag, Tag* endTag,QUrl* base);
        QUrl getUrl(){return url;}
        QUrl urlFromHref (Tag* tag, QUrl* base);
        void setText (const QString& text) {this->text = text;}
        static QUrl resolved(QUrl* base,const string& relative);
        int getDepth(){return depth;}
        QString getText(){return text;}
        virtual ~Link(void);

        int id;
};

#endif // LINK_H
