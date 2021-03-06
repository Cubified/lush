/*
 * lush.h: static definitions
 */

#ifndef __LUSH_H
#define __LUSH_H

enum text_colors {
  CMD_INVALID = 167,
  CMD_VALID = 176,
  STRING = 3,
  PIPE = 165,
  ARG = 74
};

char PS1[255];
int running = 1,
    PS1_len;

void sighandler(int sig);
void gen_ps1(char *arg);

char *trim(char *inp, int direction);
char *tok(char *inp);

int check_builtins(char *cmd, int check_only, char *full);
int determine_color(char *start, char *current, char *next, char *prev);

void onion_skin(char *full, char *current, char *next, int ind, int do_append);
void syntax(char *inp, int is_final);

#endif
