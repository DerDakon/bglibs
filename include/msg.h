#ifndef MSG__H__
#define MSG__H__

#include "sysdeps.h"
#include <stdarg.h>
#include "debug.h"

extern const char program[] __attribute__((__weak__));
extern const int msg_show_pid __attribute__((__weak__));

void msg_common(const char* type, const char*, const char*, const char*, const char*, const char*, const char*, int);
void msg_commonf(const char* type, int showsys, const char* format, ...);
void msg_commonfv(const char* type, int showsys, const char* format, va_list ap);
#define msg(a,b,c,d,e,f) msg_common(0,a,b,c,d,e,f,0)
void msg_warn(const char*, const char*, const char*, const char*, const char*, const char*, int);
void msg_warnf(int showsys, const char* format, ...);
void msg_error(const char*, const char*, const char*, const char*, const char*, const char*, int);
void msg_errorf(int showsys, const char* format, ...);
void msg_die(int code, const char* a, const char* b, const char* c, const char* d, const char* e, const char* f, int showsys) __NORETURN__;
void msg_dief(int code, int showsys, const char* format, ...) __NORETURN__;

#define msg1(a)           msg(a,0,0,0,0,0)
#define msg2(a,b)         msg(a,b,0,0,0,0)
#define msg3(a,b,c)       msg(a,b,c,0,0,0)
#define msg4(a,b,c,d)     msg(a,b,c,d,0,0)
#define msg5(a,b,c,d,e)   msg(a,b,c,d,e,0)
#define msg6(a,b,c,d,e,f) msg(a,b,c,d,e,f)
#define msgf(f,...)       msg_commonf(0,0,f,__VA_ARGS__)

#define warn1(a)           msg_warn(a,0,0,0,0,0,0)
#define warn2(a,b)         msg_warn(a,b,0,0,0,0,0)
#define warn3(a,b,c)       msg_warn(a,b,c,0,0,0,0)
#define warn4(a,b,c,d)     msg_warn(a,b,c,d,0,0,0)
#define warn5(a,b,c,d,e)   msg_warn(a,b,c,d,e,0,0)
#define warn6(a,b,c,d,e,f) msg_warn(a,b,c,d,e,f,0)
#define warnf(f,...)       msg_warnf(0,f,__VA_ARGS__)

#define error1(a)           msg_error(a,0,0,0,0,0,0)
#define error2(a,b)         msg_error(a,b,0,0,0,0,0)
#define error3(a,b,c)       msg_error(a,b,c,0,0,0,0)
#define error4(a,b,c,d)     msg_error(a,b,c,d,0,0,0)
#define error5(a,b,c,d,e)   msg_error(a,b,c,d,e,0,0)
#define error6(a,b,c,d,e,f) msg_error(a,b,c,d,e,f,0)
#define errorf(f,...)       msg_errorf(0,f,__VA_ARGS__)

#define die1(x,a)           msg_die(x,a,0,0,0,0,0,0)
#define die2(x,a,b)         msg_die(x,a,b,0,0,0,0,0)
#define die3(x,a,b,c)       msg_die(x,a,b,c,0,0,0,0)
#define die4(x,a,b,c,d)     msg_die(x,a,b,c,d,0,0,0)
#define die5(x,a,b,c,d,e)   msg_die(x,a,b,c,d,e,0,0)
#define die6(x,a,b,c,d,e,f) msg_die(x,a,b,c,d,e,f,0)
#define dief(x,f,...)       msg_dief(x,0,f,__VA_ARGS__)

#define warn1sys(a)           msg_warn(a,0,0,0,0,0,1)
#define warn2sys(a,b)         msg_warn(a,b,0,0,0,0,1)
#define warn3sys(a,b,c)       msg_warn(a,b,c,0,0,0,1)
#define warn4sys(a,b,c,d)     msg_warn(a,b,c,d,0,0,1)
#define warn5sys(a,b,c,d,e)   msg_warn(a,b,c,d,e,0,1)
#define warn6sys(a,b,c,d,e,f) msg_warn(a,b,c,d,e,f,1)
#define warnfsys(f,...)       msg_warnf(1,f,__VA_ARGS__)

#define error1sys(a)           msg_error(a,0,0,0,0,0,1)
#define error2sys(a,b)         msg_error(a,b,0,0,0,0,1)
#define error3sys(a,b,c)       msg_error(a,b,c,0,0,0,1)
#define error4sys(a,b,c,d)     msg_error(a,b,c,d,0,0,1)
#define error5sys(a,b,c,d,e)   msg_error(a,b,c,d,e,0,1)
#define error6sys(a,b,c,d,e,f) msg_error(a,b,c,d,e,f,1)
#define errorfsys(f,...)       msg_errorf(1,f,__VA_ARGS__)

#define die1sys(x,a)           msg_die(x,a,0,0,0,0,0,1)
#define die2sys(x,a,b)         msg_die(x,a,b,0,0,0,0,1)
#define die3sys(x,a,b,c)       msg_die(x,a,b,c,0,0,0,1)
#define die4sys(x,a,b,c,d)     msg_die(x,a,b,c,d,0,0,1)
#define die5sys(x,a,b,c,d,e)   msg_die(x,a,b,c,d,e,0,1)
#define die6sys(x,a,b,c,d,e,f) msg_die(x,a,b,c,d,e,f,1)
#define diefsys(x,f,...)       msg_dief(x,1,f,__VA_ARGS__)

extern void die_oom(int);

#endif
