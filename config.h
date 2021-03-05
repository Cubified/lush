/*
 * config.h: lush configuration
 */

#ifndef __CONFIG_H
#define __CONFIG_H

#include "lush.h"

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
