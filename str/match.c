/* str/match.c - Simple pattern matching
 * Copyright (C) 2002  Bruce Guenter <bruceg@em.ca>
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
#include "str.h"

/** Simple but fast (linear time) pattern matching. */
int str_match(const str* s, const char* pattern)
{
  const char* sptr;
  long slen;
  char p;
  slen = s->len;
  if (*pattern == 0) return slen == 0;
  for (sptr = s->s; (p = *pattern) != 0; ++pattern, ++sptr, --slen) {
    if (p == '*') {
      if ((p = *++pattern) == 0) return 1;
      while (slen > 0 && *sptr != p) ++sptr, --slen;
      if (slen == 0) return 0;
    }
    else
      if (slen == 0 || *sptr != p) return 0;
  }
  return slen == 0;
}

#ifdef SELFTEST_MAIN
#include "selftest.c"
static const str s = { "abc", 3, 0 };
void t(const char* pattern)
{
  obuf_puts(&outbuf, pattern);
  obuf_putc(&outbuf, ' ');
  debugfn(str_match(&s, pattern));
}
MAIN
{
  t("*");
  t("**");
  t("a");
  t("a*");
  t("*a*");
  t("*a");
  t("b*");
  t("*b*");
  t("*b");
  t("c*");
  t("*c*");
  t("*c");
  t("d*");
  t("*d*");
  t("*d");
}
#endif
#ifdef SELFTEST_EXP
* result=1
** result=0
a result=0
a* result=1
*a* result=1
*a result=0
b* result=0
*b* result=1
*b result=0
c* result=0
*c* result=1
*c result=1
d* result=0
*d* result=0
*d result=0
#endif
