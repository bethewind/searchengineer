#include "indexoutput.h"
#include <qobject.h>
#include <string>
using namespace std;
IndexOutput::IndexOutput()
{
}
IndexOutput::~IndexOutput()
{
}
//Writes an int as four bytes.
void IndexOutput::writeInt(int i)
{
    writeByte((unsigned char)(i >> 24));
    writeByte((unsigned char)(i >> 16));
    writeByte((unsigned char)(i >>  8));
    writeByte((unsigned char) i);
  }
//Writes an int in a variable-length format. Writes between one and five bytes. Smaller values take fewer bytes.
//Negative numbers are not supported.
void IndexOutput::writeVInt(int vi)
{
    unsigned int i = vi;
    while ((i & ~0x7F) != 0) {
      writeByte((unsigned char)((i & 0x7f) | 0x80));
      i >>= 7;
    }
    writeByte((unsigned char)i);
 }
//
void IndexOutput::writeVLong(__int64 vi)
{
     unsigned __int64 i = vi;
    while ((i & ~0x7F) != 0) {
      writeByte((unsigned char)((i & 0x7f) | 0x80));
      i >>= 7;
    }
    writeByte((unsigned char)i);
  }
//Writes a long as eight bytes.
void IndexOutput::writeLong(__int64 i)
{
    writeInt((int) (i >> 32));
    writeInt((int) i);
  }
//
void IndexOutput::writeString(QString s)
 {
//    int length = 0;
//      if ( s != NULL )
//        length = wcslen(s);
    writeVInt(s.length());
    writeChars(s, 0, s.length());
 }
//
void IndexOutput::writeChars(QString s, int start, int length)
   {
    //QString s1 = QObject::tr(s.toStdString().c_str());
    const int end = start + length;
    for (int i = start; i < end; i++)
    {
      const int code = (int)s[i].unicode();
      if (code >= 0x01 && code <= 0x7F)
        writeByte((unsigned char)code);
      else if (((code >= 0x80) && (code <= 0x7FF)) || code == 0)
      {
        writeByte((unsigned char)(0xC0 | (code >> 6)));
        writeByte((unsigned char)(0x80 | (code & 0x3F)));
      } else
      {
        writeByte((unsigned char)(0xE0 | (((unsigned int)code) >> 12)));
        writeByte((unsigned char)(0x80 | ((code >> 6) & 0x3F)));
        writeByte((unsigned char)(0x80 | (code & 0x3F)));
      }
    }
  }
//
void IndexOutput::writeBytes(unsigned char* b, int length)
{
    writeBytes(b, 0, length);
}
