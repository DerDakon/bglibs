/* net/tcp4 - Create an IPv4 TCP socket
 * Copyright (C) 2004,2005  Bruce Guenter <bruce@untroubled.org>
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
#include <sys/types.h>
#include <sys/param.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include "socket.h"

/** Create a TCP/IPv4 socket. */
int socket_tcp4(void)
{
  return socket(AF_INET, SOCK_STREAM, 0);
}

/** Create a TCP/IP socket. */
int socket_tcp(void)
{
  return socket_tcp4();
}

#ifdef SELFTEST_MAIN
#include <sys/stat.h>
MAIN
{
  int fd;
  struct stat st;
  debugsys((fd = socket_tcp4()) >= 0);
  debugsys(fstat(fd, &st));
  debugfn((st.st_mode & S_IFMT) == S_IFSOCK);
}
#endif
#ifdef SELFTEST_EXP
result=1
result=0
result=1
#endif
