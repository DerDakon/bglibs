/* net/connect6.c - Make an IPv6 connection
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
#include <unistd.h>
#include "socket.h"

/** Connect a socket to an IPv6 address. */
int socket_connect6(int sock, const ipv6addr* ip, ipv6port port)
{
#if HASIPV6
  struct sockaddr_in6 sa;
  memset(&sa, 0, sizeof sa);
  sa.sin6_family = AF_INET6;
  memcpy((char*)&sa.sin6_addr, &ip->addr, 16);
  sa.sin6_port = htons(port);
  return connect(sock, (struct sockaddr*)&sa, sizeof sa) == 0;
#else
  errno = EPROTONOSUPPORT;
  return 0;
#endif
}