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
#include "terminal_dir.h"
#define gcdr getcwd
#define _XOPEN_SOURCE 500
int m  = 0;

int goto_func(char **args,char ch[], int p,char root[])
{
  if (args[1] == NULL) {
    fprintf(stderr, "Give folder name to goto\n");
  } else {
    if(args[1][0]=='/')
    {
      chdir(ch);
    }
  else if (chdir(args[1]) != 0) {
      perror("error");
    }
  }
  return 1;
}


void func2(char *rootpath,FILE* fp,char *ch1)
{
  DIR *dir = opendir(rootpath);
  struct dirent *dp;
    while ((dp = readdir(dir)) != NULL)
    {
      if (strcmp(dp->d_name, ".") != 0 && strcmp(dp->d_name, "..") != 0)
      {
      freopen(ch1,"a+",fp);
      if(m==0)
        fprintf(fp,"\n\n%-20s",dp->d_name);
      if(m%6==0&&m!=0)
        fprintf(fp,"\n%-20s",dp->d_name);
      else
        fprintf(fp,"%-20s",dp->d_name);
      m++;
    }
  }
      closedir(dir);
}

void func1(char *rootpath,FILE* fp,char *ch1)
{
  char pth[1000];
  DIR *dir = opendir(rootpath);
  struct dirent *dp;
  if(dir)
  {
    while ((dp = readdir(dir)) != NULL)
    {
      if (strcmp(dp->d_name, ".") != 0 && strcmp(dp->d_name, "..") != 0)
      {
      if(dp->d_type == DT_DIR)
      {
        m=0;
    freopen(ch1,"a+",fp);
    fprintf(fp,"\n\n./%s:\n",dp->d_name);
    strcpy(pth, rootpath);
    strcat(pth, "/");
    strcat(pth,dp->d_name);
    func2(pth,fp,ch1);
    func1(pth,fp,ch1);
  }
  }
}
}
  closedir(dir);
}

void reclist(char *rootpath,FILE* fp,char *ch1)
{
    int i;
    char path[1000];
    struct dirent *dp;
    DIR *dir = opendir(rootpath);
    if (!dir)
        return;
    while ((dp = readdir(dir)) != NULL)
    {
        if (strcmp(dp->d_name, ".") != 0 && strcmp(dp->d_name, "..") != 0)
        {
            char *cm;
	          freopen(ch1,"a+",fp);
            if(m==0)
              fprintf(fp,".:\n%-20s",dp->d_name);
            if(m%6==0&&m!=0)
              fprintf(fp,"\n%-20s",dp->d_name);
            else
              fprintf(fp,"%-20s",dp->d_name);
            m++;
            strcpy(path, rootpath);
            strcat(path, "/");
            strcat(path, dp->d_name);
            reclist(path,fp,ch1);
        }
    }
    closedir(dir);
}

int snapshot_func(char **args,char ch[],int p,char root[]){
	char ch1[FILENAME_MAX], ch2[FILENAME_MAX],ch3[FILENAME_MAX];
	strcpy(ch1,ch);
	ch2[0]='/';
	ch2[1]='\0';
	getcwd(ch3,sizeof(ch3));
	chdir(ch);
	strcat(ch2,args[2]);
	strcat(ch1,ch2);
  FILE* fp = fopen(ch1,"w+");
	reclist(args[1],fp,ch1);
  func1(args[1],fp,ch1);
	chdir(ch3);
  return 1;
}
