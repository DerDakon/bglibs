#include <errno.h>
#include <unistd.h>
#include "obuf.h"

/** Flush the \c obuf and then \c fsync the file descriptor. */
int obuf_sync(obuf* out)
{
  iobuf* io;
  if (!obuf_flush(out)) return 0;
  io = &out->io;
  if (fsync(io->fd) == -1) IOBUF_SET_ERROR(io);
  return 1;
}
