/*
 * config.h: lush configuration
 */

#ifndef __CONFIG_H
#define __CONFIG_H

#include "lush.h"

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
struct passwd *pw;
int pw_dir_len;
char hostname[255];

void gen_ps1(char *arg){
  int i = 0;
  char cwd[255], arg_trim[255];

  if(pw == NULL){
    pw = getpwuid(getuid());
    pw_dir_len = strlen(pw->pw_dir);
    gethostname(hostname, 255);
  }

  if(arg[0] == '\0' || arg[0] == '\n'){
    memcpy(arg_trim, pw->pw_dir, pw_dir_len);
    arg_trim[pw_dir_len] = '\0';
  } else realpath(arg, arg_trim);

  if(chdir(arg_trim) < 0){
    printf("\x1b[38;5;%imlush: Could not change directory to \x1b[38;5;%im%s." COLOR_RESET "\n", CMD_INVALID, STRING, arg_trim);
    return;
  }

  getcwd(cwd, 255);
  if(strncmp(cwd, pw->pw_dir, pw_dir_len) == 0){
    cwd[0] = '~';
    memmove(cwd+1, cwd+pw_dir_len, strlen(cwd)-pw_dir_len+1);
  }
  PS1_len = sprintf(PS1, "\x1b[33m%s\x1b[0m@\x1b[35m%s\x1b[0m:\x1b[34m%s\x1b[0m$ ", getlogin(), hostname, cwd)-27;
}

/*
 * HISTORY FILE
 */
#define HISTFILE "/tmp/lush_history"

/*
 * ALIASES
 */
char *aliases_from[] = {
  "ls"
};

char *aliases_to[] = {
  "ls -alh --color"
};

/* TODO: Determine this at compile time
 * (may not be possible because of array weirdness)
 */
int aliases_len[] = {
  2
};

/*
 * BUILTINS
 */
char *builtins_from[] = {
  "exit",
  "cd"
};

void (*builtins_to[])(char*) = {
  cleanup,
  gen_ps1
};

int builtins_len[] = {
  4,
  2
};

#endif
