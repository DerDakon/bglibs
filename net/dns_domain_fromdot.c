#include <errno.h>
#include <stdlib.h>
#include <string.h>

#include "dns.h"

/** Generate a DNS encoded domain name into \p out from the dotted string in \c buf. */
int dns_domain_fromdot(char **out,const char *buf,unsigned int n)
{
  char label[63];
  unsigned int labellen = 0; /* <= sizeof label */
  char name[255];
  unsigned int namelen = 0; /* <= sizeof name */
  char ch;
  char *x;

  errno = EPROTO;

  for (;;) {
    if (!n) break;
    ch = *buf++; --n;
    if (ch == '.') {
      if (labellen) {
	if (namelen + labellen + 1 > sizeof name) return 0;
	name[namelen++] = labellen;
	memcpy(name + namelen,label,labellen);
	namelen += labellen;
	labellen = 0;
      }
      continue;
    }
    if (ch == '\\') {
      if (!n) break;
      ch = *buf++; --n;
      if ((ch >= '0') && (ch <= '7')) {
	ch -= '0';
	if (n && (*buf >= '0') && (*buf <= '7')) {
	  ch <<= 3;
	  ch += *buf - '0';
	  ++buf; --n;
	  if (n && (*buf >= '0') && (*buf <= '7')) {
	    ch <<= 3;
	    ch += *buf - '0';
	    ++buf; --n;
	  }
	}
      }
    }
    if (labellen >= sizeof label) return 0;
    label[labellen++] = ch;
  }

  if (labellen) {
    if (namelen + labellen + 1 > sizeof name) return 0;
    name[namelen++] = labellen;
    memcpy(name + namelen,label,labellen);
    namelen += labellen;
    labellen = 0;
  }

  if (namelen + 1 > sizeof name) return 0;
  name[namelen++] = 0;

  x = malloc(namelen);
  if (!x) return 0;
  memcpy(x,name,namelen);

  if (*out) free(*out);
  *out = x;
  return 1;
}