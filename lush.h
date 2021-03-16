/*
 * lush.h: static definitions
 */

#ifndef __LUSH_H
#define __LUSH_H

#include <signal.h>
#include <libgen.h>

#ifdef HAS_FZF
#  define ONIONSKIN_CMD "find %s -maxdepth 1 -mindepth 1 2>/dev/null | fzf -i -f '%s'"
#  define ONIONSKIN_PRINTF() printf(COLOR_ONIONSKIN "\x1b[%liD%s", strlen(usr), buf+strlen(tok)+1)
#else
#  define ONIONSKIN_CMD "find %s -iname '%s*' -maxdepth 1 -mindepth 1 -print -quit 2>/dev/null"
#  define ONIONSKIN_PRINTF() printf(COLOR_ONIONSKIN "%s", buf+strlen(tok)+strlen(usr)+1)
#endif
#define VALIDTEST_CMD "command -v %s > /dev/null 2>&1"

/* Pseudo-closure C idiom */
#define EXEC_ONIONSKIN() \
  do { \
    sprintf(cmd, ONIONSKIN_CMD, tok, usr); \
    fd = popen(cmd, "r"); \
    if(fgets(buf, sizeof(buf), fd) != NULL){ \
      buf[strlen(buf)-1] = '\0'; \
      if(do_append == -1){ \
        memcpy(out+out_len-strlen(usr), buf+strlen(tok)+1, strlen(buf)-strlen(tok)-1); \
        strcat(out, " "); \
      } else ONIONSKIN_PRINTF(); \
      pclose(fd); \
      return; \
    } \
    pclose(fd); \
  } while(0)
#define ONIONSKIN_DIRSEARCH() \
  do { \
    strcpy(path, usr); \
    realpath(dirname(path), tok); \
    strcpy(path, usr); \
    strcpy(usr, basename(path)); \
    EXEC_ONIONSKIN(); \
  } while(0)
#define ONIONSKIN_PATHSEARCH() \
  do { \
    char *path_s = getenv("PATH"); \
    strcpy(path, path_s); \
    strtok(path, ":"); \
    while((tok=strtok(NULL, ":")) != NULL){ \
      EXEC_ONIONSKIN(); \
    } \
  } while(0)

#define LUSH_BUFSIZE 256

char PS1[LUSH_BUFSIZE];
int running = 1,
    PS1_len;

void sighandler();

char *trim(char *inp);
char *tok(char *inp);

int check_builtins(char *cmd, int check_only);
int determine_color(char *current, char *next, char *prev);

void onion_skin(char *current, char *next, int ind, int do_append);
void syntax(int is_final);

void parse_histfile();
void transform_output();

void cleanup();

#endif
