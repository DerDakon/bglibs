#include <iobuf/iobuf.h>
#include <str/str.h>

/** Read the remainder of the \c ibuf into the \c str. */
int ibuf_readall(ibuf* in, str* out)
{
  for (;;) {
    if (!str_catb(out,
		  in->io.buffer+in->io.bufstart,
		  in->io.buflen-in->io.bufstart))
      return 0;
    in->io.bufstart = in->io.buflen;
    if (!ibuf_refill(in))
      return ibuf_eof(in);
  }
}
