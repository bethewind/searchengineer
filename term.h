#ifndef TERM_H
#define TERM_H
#include <QString>
class Term
{
public:
    Term();
    Term(QString fld, QString txt);
    void set(QString fld, QString txt);
    int compareTo(Term* other);
    bool operator== (Term* other);

    QString field;
     QString text;
};

#endif // TERM_H
