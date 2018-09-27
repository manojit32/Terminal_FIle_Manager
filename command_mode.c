//Manojit Chakraborty
//2018201032

#include <ftw.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <errno.h>
#include <pwd.h>
#include "terminal_cdr.h"
#include "terminal_alter.h"
#include "terminal_dir.h"


#define gcdr getcwd
#define _XOPEN_SOURCE 500
#define TOKEN_BUFFERSIZE 64
#define DELIMITER " \t\r\n\a"
#define MY_BUFFERSIZE 1024

int p, indent = 0;
char ch[FILENAME_MAX],root [FILENAME_MAX];
char *strings[] = {
  "goto",
  "exit",
  "delete",
  "rename",
  "move",
  "create_file",
  "create_dir",
  "copy",
  "snapshot"
};


int exit_func(char **args,char ch[],int p,char[]);

int (*functions[]) (char **,char*,int,char[]) = {
  &goto_func,
  &exit_func,
  &delete_func,
  &rename_func,
  &move_func,
  &createfile_func,
  &createdir_func,
  &copy_func,
  &snapshot_func
};


int num_functions() {
  return sizeof(strings) / sizeof(char *);
}

int exit_func(char **args,char ch[],int p,char root[])
{
	return 0;
}

int mainfunc(char **argv){
  gcdr(ch, sizeof(ch));
  return EXIT_SUCCESS;
}


int new_term(char **args)
{
  pid_t pid, wpid;
  int prgstat;
  pid = fork();
  if (pid == 0) {
    if (execvp(args[0], args) == -1) {
      perror("Filemanager error");
    }
    exit(EXIT_FAILURE);
  } else if (pid < 0) {
    perror("error");
  } else {

    do {
      wpid = waitpid(pid, &prgstat, WUNTRACED);
    } while (!WIFEXITED(prgstat) && !WIFSIGNALED(prgstat));
  }
  return 1;
}



int execute_func(char **args)
{
  int i;
  if (args[0] == NULL) {
    return 1;
  }
  for (i = 0; i < num_functions(); i++) {
    if (strcmp(args[0], strings[i]) == 0) {
      int in=(*functions[i])(args,ch,p,root);
      return 0;
    }
  }
  return new_term(args);
}




char *read_command(void)
{
  int bufsize = MY_BUFFERSIZE,my_pos = 0,c;
  char *buffer = malloc(sizeof(char) * bufsize);
  if (!buffer) {
    fprintf(stderr, "allocation error\n");
    exit(EXIT_FAILURE);
  }

  while (1) {
    c = getchar();
    if (c == EOF || c == '\n') {
      buffer[my_pos] = '\0';
      return buffer;
    } else {
      buffer[my_pos] = c;
    }
    my_pos++;
    if (my_pos >= bufsize) {
      bufsize += MY_BUFFERSIZE;
      buffer = realloc(buffer, bufsize);
      if (!buffer) {
        fprintf(stderr, "allocation error\n");
        exit(EXIT_FAILURE);
      }
    }
  }
}



char **split_command(char *line)
{
  int bufsize = TOKEN_BUFFERSIZE, my_pos = 0;
  char **toks = malloc(bufsize * sizeof(char*));
  char *tok;

  if (!toks) {
    exit(EXIT_FAILURE);
  }
  tok = strtok(line, DELIMITER);
  while (tok != NULL) {
    toks[my_pos] = tok;
    my_pos++;
    if (my_pos >= bufsize) {
      bufsize += TOKEN_BUFFERSIZE;
      toks = realloc(toks, bufsize * sizeof(char*));
      if (!toks) {
        exit(EXIT_FAILURE);
      }
    }
    tok = strtok(NULL, DELIMITER);
  }
  p=my_pos;
  toks[my_pos] = NULL;
  return toks;
}


int start_loop(char* line,char *root1)
{
  int jj;
  char **args;
  int prgstat;
  gcdr(root, sizeof(root));
  for(jj=0;jj<=strlen(root1);jj++){
    ch[jj]=root1[jj];
  }
  do {
      char crp[FILENAME_MAX];
     gcdr(crp, sizeof(crp));
    crp[sizeof(crp) - 1] = '\0';
    args = split_command(line);
    prgstat = execute_func(args);
  } while (prgstat);
  return 1;
}
