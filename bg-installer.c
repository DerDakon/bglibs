#include "sysdeps.h"
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include "cli/cli.h"
#include "iobuf/ibuf.h"
#include "iobuf/obuf.h"
#include "iobuf/iobuf.h"
#include "msg/msg.h"
#include "str/str.h"

static int opt_verbose = 0;
static int opt_dryrun = 0;
static int opt_check = 0;

const char program[] = "bg-installer";
const int msg_show_pid = 0;
const char cli_help_prefix[] = "\n";
const char cli_help_suffix[] =
"See http://untroubled.org/bglibs/docs/group__installer.html\n"
"for more information\n";
const char cli_args_usage[] = "top-directory <LIST";
const int cli_args_min = 1;
const int cli_args_max = 1;
cli_option cli_options[] = {
  { 'v', "verbose", CLI_FLAG, 1, &opt_verbose,
    "List the files as they are installed", 0 },
  { 'n', "dry-run", CLI_FLAG, 1, &opt_dryrun,
    "Don't modify any files", 0 },
  { 'c', "check", CLI_FLAG, 1, &opt_check,
    "Check destinations instead of installing", 0 },
  {0,0,0,0,0,0,0}
};

/** \defgroup installer bg-installer: Standardzied installer program

The program \c bg-installer reads an installation instruction file from
standard input, and executes the instructions in the directory named on
the command line.

Each line in the file is an individual instruction.  Blank lines and
lines beginning with \c # are ignored.  All other lines must have
following format:

<tt>CMD:[UID]:[GID]:MODE:DIR[:FILENAME[:SOURCE]]</tt>

- CMD is a single character specifying the installation command.
- UID (optional) is the owner ID for the target file
- GID (optional) is the group ID for the target file
- MODE is the permissions of the installed file in octal.
- DIR is the subdirectory of the installation prefix for the destination file.
- FILENAME (optional) is the name of the file to install into the
  directory.  It may be omitted for directories.
- SOURCE (optional) is the name of the source file to install, if it
  differs from the destination file, or the path in the symlink.

The target filename is \c PREFIX/TOP/DIR/FILENAME where \c PREFIX is the
value of \c $install_prefix if it is set, \c TOP is the directory given
on the command line, and \c DIR and \c FILENAME given in the
installation file.

- c lines copy regular files from the current directory
- d lines create directories (\c FILENAME is ignored)
- s lines create symlinks (\c UID, \c GID, and \c MODE are ignored)
*/

static const char* prefix;
static const char* topdir;
static unsigned lineno = 0;
static str path;
static int errors = 0;

static int getint(const char* s, unsigned* i, int base, const char* desc)
{
  char* end;
  if (*s == 0)
    *i = -1;
  else {
    *i = strtoul(s, &end, base);
    if (*end != 0) {
      warnf("{Line #}u{ has invalid }s{ number, skipping\n}", lineno, desc);
      return 0;
    }
  }
  return 1;
}

static void makepath(const char* dir, const char* file)
{
  path.len = 0;
  if (prefix != 0)
    str_copys(&path, prefix);
  str_joins(&path, '/', topdir);
  str_joins(&path, '/', dir);
  if (file != 0)
    str_joins(&path, '/', file);
}

static void error(const char* msg)
{
  errorf("{Path '}s{' }s", path.s, msg);
  ++errors;
}

static void show(char type, unsigned uid, unsigned gid, unsigned mode,
		 const char* a, const char* b)
{
  static const char UID_NA[] = "      N/A";

  if (!opt_verbose)
    return;
  
  obuf_putf(&outbuf, "cccccccccc",
	    type,
	    (mode & 0400) ? 'r' : '-',
	    (mode & 0200) ? 'w' : '-',
	    (mode & 0100)
	    ? (mode & 04000) ? 's' : 'x'
	    : (mode & 04000) ? 'S' : '-',
	    (mode & 040) ? 'r' : '-',
	    (mode & 020) ? 'w' : '-',
	    (mode & 010)
	    ? (mode & 02000) ? 's' : 'x'
	    : (mode & 02000) ? 'S' : '-',
	    (mode & 04) ? 'r' : '-',
	    (mode & 02) ? 'w' : '-',
	    (mode & 01)
	    ? (mode & 01000) ? 't' : 'x'
	    : (mode & 01000) ? 'T' : '-');

  if (uid == (unsigned)-1)
    obuf_puts(&outbuf, UID_NA);
  else
    obuf_putf(&outbuf, "\\ 8d", uid);
  if (gid == (unsigned)-1)
    obuf_puts(&outbuf, UID_NA);
  else
    obuf_putf(&outbuf, "\\ 8d", gid);

  obuf_putc(&outbuf, ' ');
  obuf_putstr(&outbuf, &path);
  if (a != 0)
    obuf_puts(&outbuf, a);
  if (b != 0)
    obuf_puts(&outbuf, b);
  obuf_endl(&outbuf);
}

static void setmode(unsigned uid, unsigned gid, unsigned mode)
{
  if (chown(path.s, uid, gid) != 0)
    diefsys(1, "{Could not set owner on '}s{'}", path.s);
  if (chmod(path.s, mode) != 0)
    diefsys(1, "{Could not set permissions on '}s{'}", path.s);
}

static void checkmode(unsigned uid, unsigned gid,
		      unsigned mode, unsigned type)
{
  struct stat st;
  if (stat(path.s, &st) == -1) {
    if (errno == ENOENT)
      error("does not exist");
    else
      diefsys(1, "{Could not stat '}s{'}", path.s);
  }
  else {
    if (uid != (unsigned)-1 && st.st_uid != uid)
      error("has wrong UID");
    if (gid != (unsigned)-1 && st.st_gid != gid)
      error("has wrong GID");
    if ((st.st_mode & 07777) != mode)
      error("has wrong permissions");
    if ((st.st_mode & ~07777) != type)
      error("is wrong file type");
  }
}

static void c(unsigned uid, unsigned gid, unsigned mode, const char* src)
{
  ibuf in;
  obuf out;
  show('-', uid, gid, mode, " <= ", src);

  if (!opt_dryrun) {
    if (unlink(path.s) != 0
	&& errno != ENOENT)
      diefsys(1, "{Could not remove '}s{'}", path.s);
    if (!ibuf_open(&in, src, 0))
      diefsys(1, "{Could not open '}s{'}", src);
    if (!obuf_open(&out, path.s, OBUF_CREATE|OBUF_EXCLUSIVE, 0600, 0))
      diefsys(1, "{Could not create '}s{'}", path.s);
    if (!iobuf_copy(&in, &out)
	|| !obuf_close(&out))
      diefsys(1, "{Could not write '}s{'}", path.s);
    ibuf_close(&in);
    setmode(uid, gid, mode);
  }

  if (opt_check) {
    checkmode(uid, gid, mode, S_IFREG);
  }
}

static void d(unsigned uid, unsigned gid, unsigned mode)
{
  struct stat st;
  show('d', uid, gid, mode, 0, 0);

  if (!opt_dryrun) {
    if (stat(path.s, &st) != 0) {
      if (mkdir(path.s, 0700) == -1)
	diefsys(1, "{Could not make directory '}s{'}", path.s);
    }
    else if (!S_ISDIR(st.st_mode))
      dief(1, "{Path '}s{' exists but is not a directory}", path.s);
    setmode(uid, gid, mode);
  }

  if (opt_check) {
    checkmode(uid, gid, mode, S_IFDIR);
  }
}

static void s(const char* src)
{
  char buf[4096];
  int len;

  if (src == 0) {
    warnf("{Line #}u{ is missing link source\n}", lineno);
    return;
  }

  show('l', -1, -1, 0777, " -> ", src);

  if (!opt_dryrun) {
    if (unlink(path.s) != 0
	&& errno != ENOENT)
      diefsys(1, "{Could not remove '}s{'}", path.s);
    if (symlink(src, path.s) != 0)
      diefsys(1, "{Could not create symlink '}s{'}", path.s);
  }

  if (opt_check) {
    if ((len = readlink(path.s, buf, sizeof buf)) == -1) {
      if (errno == ENOENT)
	error("does not exist");
      else
	diefsys(1, "{Could not read symlink '}s{'}", path.s);
    }
    else
      if (len != (int)strlen(src)
	  || memcmp(buf, src, len) != 0)
	error("contains wrong symlink");
  }
}

int cli_main(int argc, char* argv[])
{
  static str line;

  topdir = argv[0];
  if (topdir[0] != '/')
    die1(1, "Top directory must start with a slash");
  prefix = getenv("install_prefix");
  if (opt_check)
    opt_dryrun = 1;
  
  while (ibuf_getstr(&inbuf, &line, LF)) {
    const char* uidstr = 0;
    const char* gidstr = 0;
    const char* modestr = 0;
    const char* dir = 0;
    const char* file = 0;
    const char* src = 0;
    unsigned uid;
    unsigned gid;
    unsigned mode;
    int i;

    ++lineno;
    str_rstrip(&line);
    if (line.len == 0 || line.s[0] == '#')
      continue;

    str_subst(&line, ':', 0);
    if ((i = str_findfirst(&line, 0)) > 0) {
      uidstr = line.s + ++i;
      if ((i = str_findnext(&line, 0, i)) > 0) {
	gidstr = line.s + ++i;
	if ((i = str_findnext(&line, 0, i)) > 0) {
	  modestr = line.s + ++i;
	  if ((i = str_findnext(&line, 0, i)) > 0) {
	    dir = line.s + ++i;
	    if ((i = str_findnext(&line, 0, i)) > 0) {
	      file = line.s + ++i;
	      if ((i = str_findnext(&line, 0, i)) > 0)
		src = line.s + ++i;
	    }
	  }
	}
      }
    }

    if (dir == 0
	|| (line.s[0] != 'd' && file == 0)) {
      warnf("{Line #}u{ is missing required fields\n}", lineno);
      continue;
    }
    if (!getint(uidstr, &uid, 10, "UID"))
      continue;
    if (!getint(gidstr, &gid, 10, "GID"))
      continue;
    if (!getint(modestr, &mode, 8, "permissions"))
      continue;
    makepath(dir, file);

    switch (line.s[0]) {
    case 'c': c(uid, gid, mode, src == 0 || *src == 0 ? file : src); break;
    case 'd': d(uid, gid, mode); break;
    case 's': s(src); break;
    default:
      warnf("{Line #}u{ has invalid command letter, skipping\n}", lineno);
    }
  }

  if (errors > 0)
    dief(1, "d{ errors were detected}", errors);
  return 0;
  (void)argc;
}