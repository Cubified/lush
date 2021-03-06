/*
 * config.h: lush configuration
 */

#ifndef __CONFIG_H
#define __CONFIG_H

#include "lush.h"

/* 
 * SYNTAX HIGHLIGHTING/ONION SKIN
 */
#define ONIONSKIN_CMD "find %s -iname '%s*' -maxdepth 1 -print -quit"
#define VALIDTEST_CMD "command -v %s > /dev/null 2>&1"

/*
 * COLORS
 */
#define COLOR_ONIONSKIN "\x1b[38;5;8m"
#define COLOR_HELPTEXT1 "\x1b[38;5;175m"
#define COLOR_HELPTEXT2 "\x1b[38;5;80m"
#define COLOR_HELPTEXT3 "\x1b[38;5;3m"
#define COLOR_HELPTEXT4 "\x1b[38;5;75m"
#define COLOR_RESET     "\x1b[0m"

enum text_colors {
  CMD_INVALID = 167,
  CMD_VALID = 176,
  STRING = 3,
  PIPE = 165,
  ARG = 74
};

/*
 * ALIASES AND BUILTINS
 */
char *aliases[] = {
  "ls", "ls -alh --color",
  NULL
};

char *builtins[] = {
  "exit",
  "cd",
  NULL
};

int (*builtins_func[])(char*) = {
  exit,
  gen_ps1,
  NULL
};

#endif
