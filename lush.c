/*
 * lush.c: a shell
 */

#include <signal.h>

#define LEDIT_HIGHLIGHT syntax
#include "ledit.h"

#include "config.h"

#define EXEC_ONIONSKIN() \
  do { \
    sprintf(cmd, ONIONSKIN_CMD, tok, usr); \
    fd = popen(cmd, "r"); \
    if(fgets(buf, sizeof(buf), fd) != NULL){ \
      buf[strlen(buf)-1] = '\0'; \
      if(do_append == -1){ \
        memcpy(full+strlen(full)-strlen(current), buf+strlen(tok)+1, strlen(buf)-strlen(tok)-1); \
        strcat(full, " "); \
      } else printf(COLOR_ONIONSKIN "%s", buf+strlen(tok)+strlen(usr)+1); \
      pclose(fd); \
      if(path != NULL) free(path); \
      return; \
    } \
    pclose(fd); \
  } while(0)

void sighandler(int sig){
  cur = 0;
  memset(out, '\0', LEDIT_MAXLEN);
  printf("\n%s", PS1);
  fflush(stdout);
}

/*
 * Note: *outlen will contain the length
 * of the string _without escape sequences_
 */
void gen_ps1(char *arg){
  char hostname[255], cwd[255];
  chdir(arg);
  gethostname(hostname, 255);
  getcwd(cwd, 255);
  PS1_len = sprintf(PS1, "\x1b[33m%s\x1b[0m@\x1b[35m%s\x1b[0m:\x1b[34m%s\x1b[0m$ ", getlogin(), hostname, cwd)-27;
}

char *trim(char *inp, int direction){
  char *tout = inp;

  while(*tout == ' '){
    tout += direction;
  }

  return tout;
}

/*
 * Non-destructively tokenize a string
 *
 * In: String starting from where next token
 *       should be found
 * Out: End of found token
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

int check_builtins(char *cmd, int check_only, char *full){
  int i;
  char buf[255];

  for(i=0;i<LENGTH(aliases);i+=2){
    if(aliases[i] != NULL &&
       aliases[i+1] != NULL &&
       strcmp(cmd, aliases[i]) == 0){
      if(!check_only){
        sprintf(buf, "%s %s", aliases[i+1], full+strlen(cmd));
        system(buf);
      }
      return 1;
    }
  }

  for(i=0;i<LENGTH(builtins);i++){
    if(builtins[i] != NULL &&
       strncmp(builtins[i], cmd, strlen(builtins[i])) == 0){
      if(!check_only) builtins_func[i](cmd+3);
      return 1;
    }
  }

  return 0;
}

int determine_color(char *start, char *current, char *next, char *prev){
  char cmd[255], check[255];
  int tok_len = strlen(current)-strlen(next);
  if(start == current ||
     prev[0] == '|'){
    strncpy(cmd, current, tok_len);
    cmd[tok_len] = '\0';
    if(check_builtins(cmd, 1, start)) return CMD_VALID;
    sprintf(check, VALIDTEST_CMD, cmd); /* TODO: Maybe avoid using system() */
    return (system(check) ? CMD_INVALID : CMD_VALID);
  }
  if(current[0] == '"' || current[0] == '\'') return STRING;
  if(current[0] == '|' || current[0] == '>') return PIPE;
  return ARG;
}

void onion_skin(char *full, char *current, char *next, int ind, int do_append){
  if(*(next-1) != ' '){
    char cmd[255],
         usr[255],
         buf[255],
         *path = NULL,
         *tok;
    FILE *fd;
    int tok_len = strlen(current)-strlen(next);
    strncpy(usr, current, tok_len);
    usr[tok_len] = '\0';
    if(ind == 0){
      char *path_s = getenv("PATH");
      path = strdup(path_s);
      tok = strtok(path, ":");
      while((tok=strtok(NULL, ":")) != NULL){
        EXEC_ONIONSKIN();
      }
      free(path);
    } else { /* TODO: If typing a filename from outside the folder, onion skin does not work */
      tok = alloca(2);
      tok[0] = '.';
      tok[1] = '\0';
      EXEC_ONIONSKIN();
    }
  }
}

void syntax(char *inp, int is_final){
  int color,
      token_ind = 0;
  char *token = inp,
       *oldtoken = NULL,
       *oldtoken_c,
       *oldertoken;
  while(*token != '\0'){
    oldertoken = oldtoken;
    oldtoken = token;
    token = tok(token);
    if(token == oldtoken) break;
    token = trim(token, 1);
    color = determine_color(inp, oldtoken, token, oldertoken);
    printf("\x1b[38;5;%im", color);
    oldtoken_c = strdup(oldtoken);
    printf("%.*s", (int)(strlen(oldtoken)-strlen(token)), oldtoken);
    if(color != CMD_VALID &&
       is_final != 1){
      onion_skin(inp, oldtoken_c, token, token_ind, is_final);
    }
    free(oldtoken_c);
    token_ind++;
  }
  fflush(stdout);
}

int main(int argc, char **argv){
  if(argc > 1){
    printf(COLOR_HELPTEXT1 "lush: " COLOR_HELPTEXT2 "a shell\n" COLOR_HELPTEXT3 "Usage: " COLOR_HELPTEXT4 "%s" COLOR_RESET "\n", argv[0]);
    exit(0);
  }

  gen_ps1(".");

  signal(SIGINT, sighandler);

  while(running){
    ledit(PS1, PS1_len);
    if(!check_builtins(out, 0, out)) system(out); /* TODO: less lazy exec */
  }

  return 0;
}
