/* str/findnextof.c - Find the next character of a set in a string
 * Copyright (C) 2001,2005  Bruce Guenter <bruce@untroubled.org>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301 USA
 */
#include "str.h"

/** Find the next instance of the given list of characters, on or after
    \c pos */
int str_findnextof(const str* s, const char* list, unsigned pos)
{
  char* p;
  int map[256];
  if (pos >= s->len) return -1;
  str_buildmap(map, list);
  for (p = s->s + pos; pos < s->len; pos++, p++)
    if (map[*(unsigned char*)p] >= 0)
      return pos;
  return -1;
}

#ifdef SELFTEST_MAIN
MAIN
{
  str s = { "01234567890123456", 16, 0 };
  obuf_puti(&outbuf, str_findnextof(&s, "654", 0)); NL();
  obuf_puti(&outbuf, str_findnextof(&s, "654", 4)); NL();
  obuf_puti(&outbuf, str_findnextof(&s, "654", 5)); NL();
  obuf_puti(&outbuf, str_findnextof(&s, "678", 9)); NL();
}
#endif
#ifdef SELFTEST_EXP
4
4
5
-1
#endif
