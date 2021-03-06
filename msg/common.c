#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include "iobuf.h"
#include "msg.h"

static pid_t pid = 0;

void msg_common(const char* type,
		const char* a, const char* b, const char* c,
		const char* d, const char* e, const char* f,
		int showsys)
{
  static int msg_show_program = -1;
  if (msg_show_program < 0)
    msg_show_program = getenv("MSG_NO_PROGRAM") == NULL;
  if (msg_show_program) {
    obuf_puts(&errbuf, program);
    if (&msg_show_pid != 0 && msg_show_pid) {
      if (!pid || msg_show_pid < 0)
	pid = getpid();
      obuf_putc(&errbuf, '[');
      obuf_putu(&errbuf, pid);
      obuf_putc(&errbuf, ']');
    }
    obuf_puts(&errbuf, ": ");
  }
  if (type) {
    obuf_puts(&errbuf, type);
    obuf_puts(&errbuf, ": ");
  }
  if (a) obuf_puts(&errbuf, a);
  if (b) obuf_puts(&errbuf, b);
  if (c) obuf_puts(&errbuf, c);
  if (d) obuf_puts(&errbuf, d);
  if (e) obuf_puts(&errbuf, e);
  if (f) obuf_puts(&errbuf, f);
  if (showsys) {
    obuf_puts(&errbuf, ": ");
    obuf_puts(&errbuf, strerror(errno));
  }
  obuf_putc(&errbuf, '\n');
  obuf_flush(&errbuf);
}

#ifdef SELFTEST_MAIN
MAIN
{
  warn2("a", "b");
}
#endif
#ifdef SELFTEST_EXP
selftest: Warning: ab
#endif
