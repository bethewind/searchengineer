#include "indexinput.h"

IndexInput::IndexInput()
{
}
//
int IndexInput::readVInt()
{
    unsigned char b = readByte();
    int i = b & 0x7F;
    for (int shift = 7; (b & 0x80) != 0; shift += 7) {
      b = readByte();
      i |= (b & 0x7F) << shift;
    }
    return i;
 }
//
int IndexInput::readInt()
{
    return ((readByte() & 0xFF) << 24) | ((readByte() & 0xFF) << 16)
         | ((readByte() & 0xFF) <<  8) |  (readByte() & 0xFF);
}
//
__int64 IndexInput::readLong()
{
    return (((__int64)readInt()) << 32) | (readInt() & 0xFFFFFFFFL);
}
//
__int64 IndexInput::readVLong()
{
    unsigned char b = readByte();
    __int64 i = b & 0x7F;
    for (int shift = 7; (b & 0x80) != 0; shift += 7) {
      b = readByte();
      i |= (b & 0x7FL) << shift;
    }
    return i;
  }
//
QString IndexInput::readString()
{
    int length = readVInt();
    chars.resize(length);
//    if (chars == 0 || length > charslength)
//    {
//      chars = new char[length];
//      charslength = length;
//    }
    readChars(chars, 0, length);
    return chars;
  }
//
void IndexInput::readChars(QString& buffer, int start, int length)
  {
    int end = start + length;
    for (int i = start; i < end; i++) {
      unsigned char b = readByte();
      if ((b & 0x80) == 0)
        buffer[i] = QChar((ushort)(b & 0x7F));
      else if ((b & 0xE0) != 0xE0) {
        buffer[i] = QChar((ushort)(((b & 0x1F) << 6)
                                   | (readByte() & 0x3F)));
      } else
        buffer[i] = QChar((ushort)(((b & 0x0F) << 12)
                                   | ((readByte() & 0x3F) << 6)
                                   |  (readByte() & 0x3F)));
    }
  }

