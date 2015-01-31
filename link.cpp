#include "link.h"
#include <new>
#include "page.h"
#include "tag.h"
#include "htmlparser.h"
#include <iostream>
Link::Link(void)
{
}
Link::Link(const QString &href, int depth,int id)
{
     new(this)Link(href);
     this->depth = depth;
     this->id = id;
}
Link::Link(const QString& href)
{
        QUrl url(href);
    new(this)Link(url);
    this->text = href;

}
Link::Link(const QUrl& url)
        :Element(new Tag(new Page(""),0,0,"",true),0)
{
 this->url = url;
         depth = 0;
         this->text = url.toString();
         this->id = 0;
}
  Link::Link (Tag* startTag, Tag* endTag,QUrl* base)
      :Element(startTag,endTag)
  {

             url = urlFromHref (startTag, base);
             //depth = startTag.getSource().getDepth() + 1;
  }
  QUrl Link::urlFromHref (Tag* tag, QUrl* base) {
          // element is a link -- make an instance of Link.

      if(base == 0)//FormButton
      {
        return QUrl(QString(""));
      }
      string hrefAttr = HtmlParser::linktag->map1.value(tag->getTagName());
          string href = tag->getHTMLAttribute(hrefAttr);
          if (tag->getTagName() == Tag::APPLET) {
              string codebase = tag->getHTMLAttribute (string("codebase"));
              if (codebase != "")
              {
//                  QUrl uuu(QString(codebase.c_str()));
//                  QString uuuhost = uuu.host();

                  base->setUrl(resolved(base,codebase).toString());


               }
          }
//          QUrl qUrl(QString(href.c_str()));
//          QString qhost = qUrl.host();

          return resolved(base,href);


      }
  QUrl Link::resolved(QUrl* base,const string& relative)
  {
      return base->resolved(QString(relative.c_str()));
//      string url = "";
//
//      if(relative.find("http://")==std::string::npos
//         && relative.find("http:/")==std::string::npos
//         && relative.find("http:")==std::string::npos
//         )
//      {
//
//          if(relative.length() > 1)
//          {
//              if((relative.at(0)=='/')&&(relative.at(1)=='/'))
//              {
//                  url = "http:" + relative;
//              }
//          }
//         else
//          {
//             string baseStr("");
//
//                  baseStr= base->toString().toStdString();
//
//             //string baseStr = "http://www.baidu.com";
//             baseStr = baseStr.substr(7,baseStr.length()-7);
//             if(baseStr.rfind('/') != std::string::npos)
//             {
//                 if(baseStr.at(baseStr.length()-1) != '/')
//                 {
//                     int ll = baseStr.rfind('/')+1;
//                     string lastStr = baseStr.substr(ll,baseStr.length()-ll);
//                     if(lastStr.find('.') == std::string::npos)
//                     {
//                         baseStr += "/";
//                     }
//                 }
//             }
//             int ptr = 0;
//             int state = 0;
//             int d = 0;//num of '.'
//             int dd = 0;//num of '../'
//             int relength = relative.length();
//
//             while(ptr < relength)
//             {
//                 if(ptr < 0) break;
//                 char c = relative.at(ptr);
//                 switch(state)
//                 {
//                 case 0:
//                     switch(c)
//                     {
//                     case '.':
//                         d++;
//                         ptr++;
//                         break;
//                     case '/':
//                         if(ptr==0)
//                         {
//                             ptr++;
//                             state = 2;
//                             break;
//                         }
//                         ptr++;
//                         if(d >= 2) dd++;
//                         d = 0;
//                         break;
//                         default:
//                         state = 1;
//                         break;
//
//
//                     }
//                     break;
//                     case 1:
//
//
//                           for(int i = 0;i < dd+1;i++)
//                           {
//                               if(baseStr.rfind('/')==std::string::npos)
//                               {
//
//                                   break;
//                               }
//                               baseStr = baseStr.substr(0,baseStr.rfind('/'));
//                           }
//
//                      url = baseStr + '/' + relative.substr(ptr,relength-ptr);
//                      ptr = -1;
//                     break;
//                     case 2:
//                      url = (baseStr.find('/')==std::string::npos) ? baseStr+'/' : baseStr.substr(0,baseStr.find('/')+1) + relative.substr(ptr,relength-ptr);
//                       ptr = -1;
//                     break;
//                 }
//             }
//             url = "http://" + url;
//         }
//
//      }
//      else
//      {
//          url = relative;
//      }
//
//
//          return QUrl(QString(url.c_str()));
  }


Link::~Link(void)
{
}
