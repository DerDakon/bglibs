#include "crc16_arc.h"

uint16 crc16_arc_update(uint16 crc, const char* data, long len)
{
  const unsigned char* ptr = data;
  while (len-- > 0)
    crc = crc16_arc_table[(crc ^ *ptr++) & 0xff] ^ (crc >> 8);
  return crc;
}

#ifdef SELFTEST_MAIN
#include "selftest.c"
MAIN
{
  obuf_putXw(&outbuf, crc16_arc_block("123456789", 9), 4, '0'); NL();
  obuf_putXw(&outbuf, crc16_arc_block("abc", 3), 4, '0'); NL();
  obuf_putXw(&outbuf, crc16_arc_block("ABC", 3), 4, '0'); NL();
  obuf_putXw(&outbuf, crc16_arc_block("This is a string", 16), 4, '0'); NL();
}
#endif
#ifdef SELFTEST_EXP
BB3D
9738
4521
4C44
#endif