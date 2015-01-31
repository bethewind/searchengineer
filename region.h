#ifndef REGION_H
#define REGION_H
#include <QMap>
#include <string>
using namespace std;
class Page;
class Region
{
protected:
        Page* source;
        int start;
        int end;
        QMap<string,string> names;//attr/value

public:
        Region(void);
        Region (Page* page, int start, int end);
        void setLabel (string name);
        void setLabel (string name,string value);
        void setObjectLabel (string name,string value);
        string getLabel (const string& name);
        string toText ();
        string toString ();
        static int findStart (const QVector<Region*>& regions, int p,int regionNum) ;
        int getStart () {return start;}
        int getEnd () {return end;}

        virtual ~Region(void);
};

#endif // REGION_H
