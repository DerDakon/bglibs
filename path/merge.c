/* path/merge.c - Merge one path into another.
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
#include <string.h>
#include "path.h"

static int path_merge_part(str* path, const char* part, unsigned long len)
{
  if (part[0] == '.') {
    /* "." parts get dropped */
    if (len == 1) return 1;
    /* ".." parts cause truncation */
    if (len == 2 && part[1] == '.') {
      int sep = str_findlast(path, '/');
      /* Don't truncate a leading '/' */
      if (sep == 0) sep = 1;
      /* If no '/' found, empty the string */
      else if (sep == -1) sep = 0;
      str_truncate(path, sep);
      return 1;
    }
  }
  if (path->len)
    if (path->s[path->len-1] != '/')
      if (!str_catc(path, '/')) return 0;
  return str_catb(path, part, len);
}

int path_merge(str* path, const char* start)
{
  /* If the path to merge in starts with a "/", drop the previous path */
  if (*start == '/')
    if (!str_copys(path, "/")) return 0;
  /* This should possibly use a striter... */
  while (*start) {
    const char* end;
    while (*start == '/') ++start;
    if ((end = strchr(start, '/')) != 0) {
      if (!path_merge_part(path, start, end - start)) return 0;
      start = end + 1;
    }
    else {
      if (!path_merge_part(path, start, strlen(start))) return 0;
      break;
    }
  }
  return 1;
}