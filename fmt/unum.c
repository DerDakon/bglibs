/* fmt/unum.c - Format an unsigned number.
 * Copyright (C) 2004  Bruce Guenter <bruceg@em.ca>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */
#include <string.h>
#include "number.h"

static unsigned rec(char* buffer, unsigned long num,
		    unsigned width, char pad,
		    unsigned base, const char* digits)
{
  char* s = buffer;
  if (width) --width;
  if (num >= base)
    s += rec(s, num/base, width, pad, base, digits);
  else {
    memset(s, pad, width);
    s += width;
  }
  *s++ = digits[num % base];
  return s - buffer;
}

/** Format an unsigned integer of arbitrary base with optional padding. */
unsigned fmt_unumw(char* buffer, unsigned long num, unsigned width, char pad,
		  unsigned base, const char* digits)
{
  char* s = buffer;
  if (buffer == 0) {
    unsigned len;
    for (len = 0; num > 0; ++len)
      num /= base;
    return len + ((width > len) ? width - len : 0);
  }
  if (num < base) {
    if (width) {
      --width;
      memset(buffer, pad, width);
      s = buffer + width;
    }
    *s++ = digits[num];
  }
  else
    s = buffer + rec(buffer, num, width, pad, base, digits);
  return s - buffer;
}

#ifdef SELFTEST_MAIN
#include "selftest.c"
static void test(unsigned (*fn)(char*, unsigned long, unsigned, char),
		 unsigned long num, unsigned width, char pad)
{
  char buf[FMT_ULONG_LEN];
  obuf_putu(&outbuf, fn(0, num, width, pad));
  obuf_putc(&outbuf, ':');
  buf[fn(buf, num, width, pad)] = 0;
  obuf_puts(&outbuf, buf);
  NL();
}

MAIN
{
  test(fmt_udecw, 10, 0,   0);
  test(fmt_udecw, 10, 1, ' ');
  test(fmt_udecw, 10, 5, ' ');
  test(fmt_udecw, 10, 5, '0');
  test(fmt_uhexw, 30, 0,   0);
  test(fmt_uhexw, 30, 5, ' ');
  test(fmt_uhexw, 30, 5, '0');
  test(fmt_uHexw, 30, 0,   0);
  test(fmt_uHexw, 30, 5, ' ');
  test(fmt_uHexw, 30, 5, '0');
}
#endif
#ifdef SELFTEST_EXP
2:10
2:10
5:   10
5:00010
2:1e
5:   1e
5:0001e
2:1E
5:   1E
5:0001E
#endif