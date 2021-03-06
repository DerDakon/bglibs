#include <sys/types.h>
#include <unistd.h>

#include "sysdeps.h"
#include "unix.h"
#include "sig.h"
#include "signalfd.h"

static int fds[2] = { -1, -1 };

static void signalfd_handler(int sig)
{
  char signum = sig;
  write(fds[1], &signum, 1);
}

/** Initialize the signal file descriptor.
 *
 * This function creates a pipe through which signals will get passed.
 * This is a way of handling signals that avoids most problems posed by
 * UNIX signals.  When a signal is caught by this interface, the
 * numerical value of the signal is passed down the pipe as a native
 * integer.  The file descriptor is set to non-blocking mode to prevent
 * blocking when reading from it.
 *
 * To use the file descriptor, use either \c poll or \c select to
 * determine when it is readable.  When it becomes readable, read a
 * single byte from it.  The value of this byte indicates what signal
 * was caught.
 *
 * Due to the global nature of UNIX signal handling, only a single
 * signalfd interface may be active at any time.  Do not call \c
 * signalfd_init multiple times without calling \c signalfd_close first.
 *
 * \return -1 if an error occurred, otherwise the return value is the
 * file descriptor opened for reading.
 */
int signalfd_init(void)
{
  if (pipe(fds) == -1)
    return -1;
  if (!cloexec_on(fds[0])
      || !nonblock_on(fds[0])
      || !cloexec_on(fds[1])
      || !nonblock_on(fds[1]))
    signalfd_close();
  return fds[0];
}

/** Mark a signal as being caught through the signalfd interface. */
void signalfd_catch(int sig)
{
  sig_catch(sig, signalfd_handler);
}

/** Unmark a signal as being caught through the signalfd interface. */
void signalfd_uncatch(int sig)
{
  sig_default(sig);
}

/** Close the signalfd interface. */
void signalfd_close(void)
{
  close(fds[0]);
  close(fds[1]);
  fds[0] = fds[1] = -1;
}

#ifdef SELFTEST_MAIN
#include <errno.h>
#include <signal.h>
void puti(const char* word, long i)
{
  obuf_put2s(&outbuf, word, ": ");
  obuf_puti(&outbuf, i);
  obuf_endl(&outbuf);
}
MAIN
{
  char buf[4];
  int fd;
  pid_t pid;

  for (fd = 3; fd < 32; ++fd)
    close(fd);
  fd = signalfd_init();
  signalfd_catch(SIGCHLD);
  puti("FD[0]", fds[0]);
  puti("FD[1]", fds[1]);
  puti("Returned FD", fd);
  puti("read before SIGCHLD", read(fd, &buf, sizeof buf));
  puti("errno is EAGAIN", errno == EAGAIN);
  pid = getpid();
  kill(pid, SIGCHLD);
  puti("read after SIGCHLD", read(fd, &buf, sizeof buf));
  puti("data is SIGCHLD", buf[0] == SIGCHLD);
  puti("second read", read(fd, &buf, sizeof buf));
  kill(pid, SIGCHLD);
  kill(pid, SIGCHLD);
  puti("read after two SIGCHLDs", read(fd, &buf, sizeof buf));
  puti("second read", read(fd, &buf, sizeof buf));
  signalfd_close();
  puti("read after close", read(fd, &buf, sizeof buf));
  puti("errno is EBADF", errno == EBADF);
}
#endif
#ifdef SELFTEST_EXP
FD[0]: 3
FD[1]: 4
Returned FD: 3
read before SIGCHLD: -1
errno is EAGAIN: 1
read after SIGCHLD: 1
data is SIGCHLD: 1
second read: -1
read after two SIGCHLDs: 2
second read: -1
read after close: -1
errno is EBADF: 1
#endif
