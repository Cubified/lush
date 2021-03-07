/*
 * config.h: lush configuration
 */

#ifndef __CONFIG_H
#define __CONFIG_H

#include "lush.h"

/* 
 * SYNTAX HIGHLIGHTING/ONION SKIN
 */
#ifdef HAS_FZF
#  define ONIONSKIN_CMD "find %s -maxdepth 1 2>/dev/null | fzf -i -f '%s'"
#else
#  define ONIONSKIN_CMD "find %s -iname '%s*' -maxdepth 1 -print -quit 2>/dev/null"
#endif
#define VALIDTEST_CMD "command -v %s > /dev/null 2>&1"

/*
 * COLORS
 */
#define COLOR_ONIONSKIN "\x1b[38;5;8m"
#define COLOR_HELPTEXT1 "\x1b[38;5;175m"
#define COLOR_HELPTEXT2 "\x1b[38;5;80m"
#define COLOR_HELPTEXT3 "\x1b[38;5;3m"
#define COLOR_HELPTEXT4 "\x1b[38;5;75m"
#define COLOR_ERROR     "\x1b[38;5;9m"
#define COLOR_RESET     "\x1b[0m"

enum text_colors {
  CMD_INVALID = 167,
  CMD_VALID = 176,
  STRING = 3,
  PIPE = 165,
  ARG = 74
};

/*
 * PROMPT
 *
 * Note: PS1_len should contain the length
 * of the string _without escape sequences_
 */
void gen_ps1(char *arg){
  int i = 0;
  char hostname[255], cwd[255], arg_trim[255];

  while(arg[i] != ' ' && arg[i++] != '\0');
  strncpy(arg_trim, arg, i);
  if(chdir(arg_trim) < 0){
    printf(COLOR_ERROR "lush: Invalid cd command" COLOR_RESET "\n");
    return;
  }

  gethostname(hostname, 255);
  getcwd(cwd, 255);
  PS1_len = sprintf(PS1, "\x1b[33m%s\x1b[0m@\x1b[35m%s\x1b[0m:\x1b[34m%s\x1b[0m$ ", getlogin(), hostname, cwd)-27;
}

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

void (*builtins_func[])(char*) = {
  cleanup,
  gen_ps1,
  NULL
};

#endif
