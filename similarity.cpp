#include "similarity.h"
#include "defaultsimilarity.h"

Similarity* Similarity::defaultImpl = new DefaultSimilarity();

union clvalue {
                int     i;
                float	f; //must use a float type, else types dont match up
        };
Similarity::Similarity()
{
}
Similarity::~Similarity()
{
}
//Encodes a normalization factor for storage in an index.
//The encoding uses a three-bit mantissa, a five-bit exponent, and the
//zero-exponent point at 15, thus representing values from around 7x10^9 to
//2x10^-9 with about one significant decimal digit of accuracy. Zero is also
//represented. Negative numbers are rounded up to zero. Values too large to r
//epresent are rounded down to the largest representable value. Positive values
//too small to represent are rounded up to the smallest positive representable
//value.

unsigned char Similarity::encodeNorm(float f)
{
    return floatToByte315(f);
}
//floatToByte(b, mantissaBits=3, zeroExponent=15)
//smallest non-zero value = 5.820766E-10
//largest value = 7.5161928E9
//epsilon = 0.125
unsigned char Similarity::floatToByte315(float f) {
    int bits = floatToIntBits(f);
    int smallfloat = bits >> (24-3);
    if (smallfloat < (63-15)<<3) {
      return (bits<=0) ? (unsigned char)0 : (unsigned char)1;
    }
    if (smallfloat >= ((63-15)<<3) + 0x100) {
      return -1;
    }
    return (unsigned char)(smallfloat - ((63-15)<<3));
 }
//
    int Similarity::floatToIntBits(float value)
        {
                clvalue u;
                int e, f;
                u.f = value;
                e = u.i & 0x7f800000;
                f = u.i & 0x007fffff;

                if (e == 0x7f800000 && f != 0)
                u.i = 0x7fc00000;

                return u.i;
        }
