#ifdef SELFTEST_MAIN
#include <stdio.h>
#include <string.h>
#include "md4.h"

static void MDString(const char* str)
{
  struct md4_ctx ctx;
  unsigned char digest[16];
  unsigned int len = strlen(str);
  unsigned i;
  
  md4_init_ctx(&ctx);
  md4_process_bytes(str, len, &ctx);
  md4_finish_ctx(&ctx, digest);
  printf("MD4 (\"%s\") = ", str);
  for (i = 0; i < 16; i++) printf("%02x", digest[i]);
  printf("\n");
}

int main(void)
{
  MDString("");
  MDString("a");
  MDString("abc");
  MDString("message digest");
  MDString("abcdefghijklmnopqrstuvwxyz");
  MDString("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789");
  
  MDString("1234567890123456789012345678901234567890"
	   "1234567890123456789012345678901234567890");
  return 0;
}
#endif
#ifdef SELFTEST_EXP
MD4 ("") = 31d6cfe0d16ae931b73c59d7e0c089c0
MD4 ("a") = bde52cb31de33e46245e05fbdbd6fb24
MD4 ("abc") = a448017aaf21d8525fc10ae87aa6729d
MD4 ("message digest") = d9130a8164549fe818874806e1c7014b
MD4 ("abcdefghijklmnopqrstuvwxyz") = d79e1c308aa5bbcdeea8ed63df412da9
MD4 ("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789") = 043f8582f241db351ce627e153e7f0e4
MD4 ("12345678901234567890123456789012345678901234567890123456789012345678901234567890") = e33b4ddc9c38f2199c3e7b164fcc0536
#endif