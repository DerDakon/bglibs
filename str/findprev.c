/* str/findprev.c - Find the previous instance of a character
 * Copyright (C) 2001  Bruce Guenter <bruceg@em.ca>
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

/** Find the previous instance of the given character on or before \c pos */
int str_findprev(const str* s, char ch, unsigned pos)
{
  char* p;
  if (pos >= s->len) pos = s->len - 1;
  for (p = s->s + pos; p >= s->s; --p)
    if (*p == ch) return p - s->s;
  return -1;
}
