/* net/getaddr6.c - Determine the IPv6 address of a socket
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
#include "sysdeps.h"
#include <errno.h>
#include <string.h>
#include <sys/param.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "socket.h"

/** Determine the IPv6 address of a socket. */
int socket_getaddr6(int sock, ipv6addr* addr, ipv6port* port)
{
#if HASIPV6
  struct sockaddr_in6 sa;
  int size;
  size = sizeof sa;
  if (getsockname(sock, (struct sockaddr*)&sa, &size) == -1) return 0;
  if (sa.sin6_family != AF_INET6) return 0;
  if (size != sizeof sa) return 0;
  memcpy(&addr->addr, &sa.sin6_addr, 16);
  *port = ntohs(sa.sin6_port);
  return 1;
#else
  errno = EPROTONOSUPPORT;
  return 0;
#endif
}