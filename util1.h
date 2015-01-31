#ifndef UTIL1_H
#define UTIL1_H
#include <QDateTime>
#include <QTime>
class Util
{
public:
    //return the currentTime millsecond than (1970.7.1 00:00:00)
    static __int64 currentTimeMillis()
    {
        QDateTime dt = QDateTime::currentDateTime();
        QTime tt = dt.time();
        int mt = tt.msec();
        __int64 t = ((__int64)dt.toTime_t())*1000 + mt;
        return t;
    }
    //Compares two strings, character by character, and returns
    //the first position where the two strings differ from one another.
    static int stringDifference(QString s1, QString s2) {
        int len1 = s1.length();
        int len2 = s2.length();
        int len = len1 < len2 ? len1 : len2;
        for (int i = 0; i < len; i++) {
          if (s1.at(i) != s2.at(i)) {
                  return i;
          }
        }
        return len;
      }

};
#endif // UTIL1_H
