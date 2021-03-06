#ifndef NET__RESOLVE__H__
#define NET__RESOLVE__H__

#include "ipv4.h"

/** \defgroup resolve resolve: Domain name resolution functions
@{ */

/** No error. */
#define RESOLVE_OK 0
/** Domain name not found. */
#define RESOLVE_NOTFOUND 1
/** Domain name exists, but has no address. */
#define RESOLVE_NOADDR 2
/** Non-recoverable name server error. */
#define RESOLVE_NSERROR 3
/** Temporary failure. */
#define RESOLVE_TEMPFAIL 4

extern const char* resolve_ipv4addr(const ipv4addr* addr);
extern int resolve_ipv4name_n(const char* name, ipv4addr* addr, int maxaddrs);
/** Compatibility macro for resolve_ipv4name_n */
#define resolve_ipv4name(N,A) resolve_ipv4name_n((N),(A),1)
extern int __resolve_error;
/** Look up the last resolve error code. */
#define resolve_error() (__resolve_error)

struct dns_result;
extern int resolve_calldns(int (*dnsfn)(), struct dns_result* out, const void* param);
extern int resolve_qualdns(int (*dnsfn)(struct dns_transmit*, struct dns_result* out, const char*),
                           struct dns_result* out, const char* name);

/** @} */

#endif
