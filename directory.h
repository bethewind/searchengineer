#ifndef DIRECTORY_H
#define DIRECTORY_H
#include <QString>
class IndexOutput;
class IndexInput;
class Directory
{
public:
    Directory();
    virtual ~Directory();
    virtual  IndexOutput* createOutput(const QString& name) = 0;
    virtual IndexInput* openInput(QString name)=0;
    virtual void close() = 0;
    virtual  bool fileExists(QString name)=0;
    virtual void deleteFile(QString name)=0;
    virtual QString getDir()=0;
private:
    void read(IndexInput* input);
};

#endif // DIRECTORY_H
