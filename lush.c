/*
 * lush.c: a shell
 */

#define LEDIT_HIGHLIGHT syntax
#include "ledit.h"
#include "config.h"

void sighandler(){
  cur = 0;
  memset(out, '\0', LEDIT_MAXLEN);
  printf("\n%s", PS1);
  fflush(stdout);
}

char *trim(char *inp){
  char *tout = inp;

  while(*tout == ' '){
    tout++;
  }

  return tout;
}

/*
 * Non-destructively tokenize a string
 *
 * In: String starting from where next token
 *       should be found
 * Out: End of found token
 *
 * TODO: Hindsight is 20/20 -- this approach is
 * about a thousand times more cumbersome than
 * just cloning the string and using strtok()
 */
char *tok(char *inp){
  int i, len = strlen(inp);
  char *tout = inp, quote = 0;
  for(i=0;i<len;i++){
    tout = &inp[i];
    switch(inp[i]){
      case ' ':
      case '\0':
        if(quote == 0) goto done;
        break;
      case '"':
      case '\'':
        if(quote == 0){
          quote = inp[i];
        } else if(quote == inp[i]){
          goto done; /* TODO: Escaping */
        }
        break;
      case '|':
      case '>':
        if(quote == 0) goto done;
        break;
    }
  }
done:;
  return tout+1;
}

int check_builtins(char *cmd, int check_only){
  long unsigned int i;
  char buf[LUSH_BUFSIZE];

  for(i=0;i<LENGTH(aliases_from);i++){
    if(strncmp(cmd, aliases_from[i], aliases_len[i]) == 0 &&
       (cmd[aliases_len[i]] == ' ' ||
        cmd[aliases_len[i]] == '\0')){
      if(!check_only) sprintf(out, "%s %s", aliases_to[i], out+aliases_len[i]);
      return 1;
    }
  }

  for(i=0;i<LENGTH(builtins_from);i++){
    if(strncmp(cmd, builtins_from[i], builtins_len[i]) == 0){
      if(!check_only) builtins_to[i](out+builtins_len[i]+1);
      return 2;
    }
  }

  return 0;
}

int determine_color(char *current, char *next, char *prev){
  char cmd[LUSH_BUFSIZE], check[LUSH_BUFSIZE];
  int tok_len = strlen(current)-strlen(next);
  if(current == out ||
     prev[0] == '|'   ||
     prev[0] == '&'){
    memcpy(cmd, current, tok_len);
    cmd[tok_len] = '\0';
    if(check_builtins(cmd, 1)) return CMD_VALID;
    sprintf(check, VALIDTEST_CMD, cmd); /* TODO: Maybe avoid using system() */
    return (system(check) ? CMD_INVALID : CMD_VALID);
  }
  if(current[0] == '"' || current[0] == '\'') return STRING;
  if(current[0] == '|' || current[0] == '>') return PIPE;
  return ARG; /* TODO: Test if argument is valid file */
}

void onion_skin(char *current, char *next, int ind, int do_append){
  if(*(next-1) != ' '){
    char cmd[LUSH_BUFSIZE],
         usr[LUSH_BUFSIZE],
         buf[LUSH_BUFSIZE],
         *path,
         *tok;
    FILE *fd;
    int tok_len = strlen(current)-strlen(next);
    memcpy(usr, current, tok_len);
    usr[tok_len] = '\0';

    tok = alloca(LUSH_BUFSIZE); /* Alloca for fun and style points */
    if(realpath(usr, tok) == NULL){
      path = alloca(LUSH_BUFSIZE);
      ONIONSKIN_HISTSEARCH();
      if(ind == 0 && !(usr[0] == '.' || usr[0] == '/')){
        ONIONSKIN_PATHSEARCH();
      } else {
        ONIONSKIN_DIRSEARCH();
      }
      return;
    }
  }
}

void syntax(int is_final){
  int color,
      token_ind = 0;
  char *token = out,
       *oldtoken = NULL,
       *oldtoken_c,
       *oldertoken;
  while(*token != '\0'){
    oldertoken = oldtoken;
    oldtoken = token;
    token = tok(token);
    if(token == oldtoken) break;
    token = trim(token);
    color = determine_color(oldtoken, token, oldertoken);
    printf("\x1b[38;5;%im", color);
    oldtoken_c = strdup(oldtoken);
    printf("%.*s", (int)(strlen(oldtoken)-strlen(token)), oldtoken);
    if(color != CMD_VALID &&
       is_final != 1){
      onion_skin(oldtoken_c, token, token_ind, is_final);
    }
    free(oldtoken_c);
    token_ind++;
  }
  fflush(stdout);
}

void parse_histfile(){
  char buf[LUSH_BUFSIZE];
  FILE *fp = fopen(HISTFILE, "r");
  if(fp == NULL) return;

  history = malloc(sizeof(char*)*history_len);
  while(fgets(buf, LEDIT_MAXLEN, fp) != NULL){
    buf[strlen(buf)-1] = '\0';
    history[history_ind++] = strdup(buf);
    if(history_ind >= history_len){
      history_len *= 2;
      history = realloc(history, sizeof(char*)*history_len);
    }
  }

  fclose(fp);
}

void transform_output(){
  char cmd[LUSH_BUFSIZE],
       *tokstart,
       *token = out;

  memset(cmd, '\0', LUSH_BUFSIZE);
  while(*token != '\0'){ /* TODO: If an alias is self-referential, this loops infinitely */
    tokstart = token;
    token = tok(token);
    memcpy(cmd, tokstart, strlen(tokstart)-strlen(token));
    if(check_builtins(cmd, 0) == 2) return;
  }
  system(out); /* TODO: Less lazy exec */
}

void cleanup(){
  int i;
  FILE *fp = fopen(HISTFILE, "w");
  for(i=0;i<history_ind;i++){
    fprintf(fp, "%s\n", history[i]); /* TODO: fprintf for a string and newline is very lazy */
    free(history[i]);
  }
  free(history);
  fclose(fp);

  exit(0);
}

int main(int argc, char **argv){
  if(argc > 1){
    printf(COLOR_HELPTEXT1 "lush: " COLOR_HELPTEXT2 "a shell\n" COLOR_HELPTEXT3 "Usage: " COLOR_HELPTEXT4 "%s" COLOR_RESET "\n", argv[0]);
    exit(0);
  }

  signal(SIGINT,  sighandler);
  signal(SIGQUIT, cleanup);
  signal(SIGKILL, cleanup);
  signal(SIGTERM, cleanup);

  if(!isatty(STDIN_FILENO)){ /* Reading from pipe, just execute commands */
    while(fgets(out, sizeof(out), stdin) != NULL) system(out);
    exit(0);
  }

  gen_ps1(".");
  parse_histfile();

  for(;;){
    ledit(PS1, PS1_len);
    transform_output();
  }

  /* Unreachable */
  return 0;
}
