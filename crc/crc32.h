#ifndef CRC32__H__
#define CRC32__H__

#include <uint32.h>

#define CRC32REVPOLY ((uint32)0xEDB88320UL)
#define CRC32INIT    ((uint32)0xFFFFFFFFUL)

extern const uint32 crc32_table[256];
extern uint32 crc32_update(uint32 crc, const char* data, long len);

#endif