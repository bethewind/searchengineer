#include "region.h"
#include "page.h"
#include <QVector>
Region::Region(void)
{
}
Region::Region (Page* page, int start, int end)
    {
        this->source = page;
        this->start = start;
        this->end = end;
    }
void Region::setLabel (string name,string value) {
        setObjectLabel (name, value);
    }
void Region::setLabel (string name) {
        setObjectLabel (name, "true");
    }
//set attr/value
void Region::setObjectLabel (string name,string value)
{

            names.insert(name, value);

    }
//
string Region::getLabel (const string& name) {

    if(names.contains(name)){return names.value(name);}
    else return "";
    }
string Region::toText () {
        return source->substringText (start, end);
    }
string Region::toString () {
        return source->substringContent (start, end);
    }
 int Region::findStart (const QVector<Region*>& regions, int p,int regionNum) {
        // returns k such that forall j<k : regions[j].start < p
        //                     && regions[k].start >= p
        int lo = 0;
        int hi = regionNum;
        // invariant: forall j<lo : regions[j].start < p
        //         && forall j>=hi : regions[j].start >= p
        while (lo != hi) {
            int mid = (hi + lo) / 2;
            if (regions[mid]->start < p)
                lo = mid+1;
            else
                hi = mid;
        }
        return hi;
    }
Region::~Region(void)
{
//        delete source;
//        source = 0;
}
