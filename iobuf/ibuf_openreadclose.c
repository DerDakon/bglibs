#include <iobuf/iobuf.h>
#include <str/str.h>

/** Open and read the entire file into the \c str . */
int ibuf_openreadclose(const char* filename, str* out)
{
  ibuf in;
  int r;
  if (!ibuf_open(&in, filename, 0))
    return -1;
  r = ibuf_readall(&in, out);
  ibuf_close(&in);
  return r;
}