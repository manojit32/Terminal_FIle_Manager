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
#define GetCurrentDir getcwd
#define _XOPEN_SOURCE 500

char ch[FILENAME_MAX];


int rename_func(char **args,char ch[],int p,char root[])
{
    int value,j1;
    char ch1[100];
    for(j1=0;j1<=strlen(ch);j1++)
    {
      ch1[j1]=ch[j1];
    }
    strcat(ch,"/");
    strcat(ch,args[1]);
    strcat(ch1,"/");
    strcat(ch1,args[2]);
    value = rename(ch,ch1);
    return 1;
}

int createfile_func(char **args,char ch[],int p,char root[])
{
  char crp[FILENAME_MAX],crp1[FILENAME_MAX],r[FILENAME_MAX],r1[FILENAME_MAX];
  strcpy(crp,ch);
  strcpy(r,args[2]);
  strcpy(crp1,crp);
  if(r[0]=='.')
  {
    strcat(crp,args[1]);
    FILE *fp;
    fp=fopen(crp,"w");
    fclose(fp);
  }
  else
  {
  if(r[0]=='~')
  {
    strcpy(crp,root);
    int j;
    for(j=1;r[j]!='\0';j++)
    {
      r1[j-1]=r[j];
    }
    r1[j-1]='\0';
    strcpy(r,r1);
}
  strcat(crp,r);
  chdir(crp);
  strcat(crp,args[1]);
  FILE *fp=fopen(crp,"w");
  chdir(crp1);
}
  return 1;
}


int createdir_func(char **args,char ch[],int p,char root[])
{
  char crp[FILENAME_MAX],crp1[FILENAME_MAX],r[FILENAME_MAX],r1[FILENAME_MAX];
  strcpy(crp,ch);
  strcpy(r,args[2]);
  strcpy(crp1,crp);
  if(r[0]=='.')
  {
    strcat(crp,args[1]);
    struct stat st = {0};
    if (stat(crp, &st) == -1) {
      mkdir(crp, 0744);
    }
   else
    {
    printf("directory already exists.\n");
    }
  }
  else
  {
  if(r[0]=='~')
  {
    strcpy(crp,ch);
    printf("%s\n",crp);
    int j;
    for(j=1;r[j]!='\0';j++)
    {
      r1[j-1]=r[j];
    }
    r1[j-1]='\0';
    strcpy(r,r1);
  }
  strcat(crp,r);
  strcpy(crp1,crp);
  strcat(crp1,args[1]);
  struct stat st = {0};
  if (stat(crp1, &st) == -1) {
    mkdir(crp1, 0744);
  }
 else
  {
  printf("directory already exists.\n");
  }
 }
  return 1;
 }

int remove_dirk(const char *path)
{
   size_t plength = strlen(path);
   int r = -1;
   DIR *d = opendir(path);
   if (d)
   {
      struct dirent *p;
      r = 0;
      while (!r && (p=readdir(d)))
      {
          char *buffer1;
          size_t len;
          int r2 = -1;
          if (!strcmp(p->d_name, ".") || !strcmp(p->d_name, ".."))
          {
             continue;
          }
          len = plength + strlen(p->d_name) + 2;
          buffer1 = malloc(len);
          if (buffer1)
          {
             struct stat statbuffer1;
             snprintf(buffer1, len, "%s/%s", path, p->d_name);
             if (!stat(buffer1, &statbuffer1))
             {
                if (S_ISDIR(statbuffer1.st_mode))
                {
                   r2 = remove_dirk(buffer1);
                }
                else
                {
                   r2 = unlink(buffer1);
                }
             }
             free(buffer1);
          }
          r = r2;
      }
      closedir(d);
   }
   if (!r)
   {
      r = rmdir(path);
   }
   return r;
}



int isFile1(const char* name)
{
    DIR* dirk = opendir(name);
    if(dirk != NULL)
    {
     closedir(dirk);
     return 0;
    }
    if(errno == ENOTDIR)
    {
     return 1;
    }
    return -1;
}

int delete_func(char **args,char ch[],int p,char root[])
{
   if (args[1] == NULL) {
    fprintf(stderr, "Give file or dir for delete\n");
  }
  else
  {
    char crp[FILENAME_MAX];
    strcpy(crp,ch);
     strcat(crp,args[1]);
     int k = isFile1(crp);
     if(k==0){
      if(remove_dirk(crp)>=0){
      }
    }
    else if(k==1){
      remove(crp);
    }
  }
   return 1;
}
