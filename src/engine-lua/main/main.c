/*
 * texpresso-luatex: LuaJIT-based TeX engine for TeXpresso (eclipse)
 *
 * I/O shim bridging LuaTeX's file system with TeXpresso protocol.
 * Provider chain: TeXpresso VFS → kpathsea/TeXlive → local filesystem.
 *
 * LuaTeX exposes Lua callbacks for file operations:
 *   find_read_file, find_write_file, find_font_file, etc.
 *   open_read_file, read_*_file
 *
 * Strategy:
 *   - Register C functions via LuaJIT FFI as Lua callbacks
 *   - find_*_file: check TeXpresso VFS first, then kpathsea
 *   - open_read_file: return protocol-backed file handle
 *   - read_*_file: txp_read with position tracking (txp_seen)
 *   - Fork injection: txp_read returns T_FORK → call txp_fork
 */

#ifdef __APPLE__
# include <sys/syslimits.h>
#else
# include <linux/limits.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>
#include "texpresso_protocol.h"

#define LOG 0

/* Global state */
static const char *primary_document = NULL;
static txp_client *texpresso = NULL;
static bool use_texpresso = 0;
static bool use_texlive = 1; /* always use kpathsea/texlive as fallback */

int main(int argc, char *argv[])
{
  const char *doc_path = NULL;
  const char *output_format = "dvi";

  for (int i = 1; i < argc; i++)
  {
    if (strcmp(argv[i], "-texpresso") == 0)
      use_texpresso = 1;
    else if (strncmp(argv[i], "--output-format=", 16) == 0)
      output_format = argv[i] + 16;
    else if (argv[i][0] != '-')
      doc_path = argv[i];
  }

  if (!doc_path)
  {
    fprintf(stderr, "Usage: texpresso-luatex [--output-format=dvi] [-texpresso] file.tex\n");
    return 1;
  }

  if (strcmp(output_format, "dvi") != 0)
  {
    fprintf(stderr, "texpresso-luatex: only DVI output mode is supported\n");
    return 1;
  }

  primary_document = doc_path;

  /* Connect to TeXpresso frontend */
  if (use_texpresso)
  {
    char *fd_str = getenv("TEXPRESSO_FD");
    if (!fd_str)
    {
      fprintf(stderr, "texpresso-luatex: TEXPRESSO_FD not set\n");
      return 1;
    }
    int fd = atoi(fd_str);
    FILE *f = fdopen(fd, "r+");
    if (!f)
    {
      perror("texpresso-luatex: fdopen");
      return 1;
    }
    texpresso = txp_connect(f);
    if (!texpresso)
    {
      fprintf(stderr, "texpresso-luatex: handshake failed\n");
      return 1;
    }
  }

  /*
   * TODO Phase 2.2: Import LuaTeX source, verify standalone build
   * TODO Phase 2.3: I/O interception
   *   - Initialize LuaJIT state
   *   - Register find_*_file callbacks (txp_open → kpathsea fallback)
   *   - Register open_read_file / read callbacks (txp_read + txp_seen)
   *   - Register write callbacks (txp_append)
   *   - Handle T_FORK responses (call txp_fork from fork.c)
   *   - Run luajittex with DVI output
   */
  fprintf(stderr, "texpresso-luatex: engine not yet integrated (stub)\n");
  return 1;
}
