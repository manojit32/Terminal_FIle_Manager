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
#include "terminal_alter.h"
#define gcdr getcwd
#define _XOPEN_SOURCE 500

int isFile33(const char* name)
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

int recursive_copy(char *filepath,char *filepath1)
 {
   DIR *d = opendir(filepath);
   size_t filepath_len = strlen(filepath), filepath_len1=strlen(filepath1), len,len1;
   struct dirent *p;
   while ((p=readdir(d))!=NULL)
   {
     char *buffer,*buffer1;
     if (!strcmp(p->d_name, ".") || !strcmp(p->d_name, ".."))
     {
        continue;
     }
     len = filepath_len + strlen(p->d_name) + 2;
     buffer = malloc(len);
     snprintf(buffer, len, "%s/%s", filepath, p->d_name);
     len1=filepath_len1 + strlen(p->d_name) +2;
     buffer1=malloc(len1);
     snprintf(buffer1, len1, "%s/%s", filepath1, p->d_name);
     if(isFile33(buffer)==0)
     {
       mkdir(buffer1, 0777);
       struct stat st;
       stat(buffer, &st);
       chmod(buffer1, st.st_mode);
       recursive_copy(buffer,buffer1);
     }
     else
     {
       FILE *source1,*source2;
       printf("%s \n %s",buffer,buffer1);
       if ((source1 = fopen(buffer,"r")) == NULL)
           {
               return 1;
           }
       if ((source2 = fopen(buffer1,"w")) == NULL)
           {
               return 1;
           }
     int c;
     while((c=fgetc(source1))!=EOF)
     {
         fputc(c,source2);
     }
     fclose(source1);
     fclose(source2);
     struct stat st;
     stat(buffer, &st);
     chmod(buffer1, st.st_mode);
     }
   }
   return 1;
 }


int move_func(char **args,char ch[],int p,char root[])
 {
   int i;
   for(i=1;i<=p-2;i++)
   {
   char crp[FILENAME_MAX],crp1[FILENAME_MAX], r[FILENAME_MAX],r1[FILENAME_MAX];
   strcpy(crp,ch);
   char *homedir;
   strcpy(crp1,ch);
   strcpy(r,args[p-1]);
   printf("%s\n",r);
   if(r[0]=='~')
   {
     strcpy(crp,root);
     printf("%s\n",crp);
     int j;
     for(j=1;r[j]!='\0';j++)
     {
       r1[j-1]=r[j];
     }
     r1[j-1]='\0';
     strcpy(r,r1);
 }
   strcat(crp1,args[i]);
   if(isFile33(crp1)==0)
   {
     strcat(r,args[i]);
   }
   else
   {
     strcat(r,args[i]);
   }
    strcat(crp,r);
    printf("%s\n",crp );
    printf("%s\n",crp1);
    rename(crp1,crp);
   }
   return 1;
 }



int copy_func(char **args,char ch[],int p,char root[])
{
 int i;
 char crp[FILENAME_MAX], crp1[FILENAME_MAX], crp2[FILENAME_MAX], r[FILENAME_MAX];
 strcpy(crp,ch);
 strcpy(crp2,crp);
 strcat(crp2,args[p-1]);
 if(isFile33(crp2)==1)
 {
   printf("Invalid destination.\n");
 }
 else
 {
 for(i=1;i<=p-2;i++)
  {
    gcdr(crp, sizeof(crp));
    strcpy(r,args[p-1]);
    if(r[0]=='~')
    {
      strcpy(crp,root);
      int j;
      char r1[FILENAME_MAX];
      for(j=1;r[j]!='\0';j++)
      {
        r1[j-1]=r[j];
      }
      r1[j-1]='\0';
      strcpy(r,r1);
    }
    strcpy(crp2,crp);
    strcat(crp2,r);
    strcpy(crp1,ch);
    strcat(crp1,args[i]);
    strcat(crp2,args[i]);
   if(isFile33(crp1)==1)
   {
     FILE *source1,*source2;
    source1 = fopen(crp1,"r");
    source2 = fopen(crp2,"w");
    int c;
   while((c=fgetc(source1))!=EOF)
   {
       fputc(c,source2);
   }
   fclose(source1);
   fclose(source2);
   struct stat st;
   stat(crp1, &st);
   chmod(crp2, st.st_mode);
  }
    else
    {
     mkdir(crp2, 0744);
     struct stat st;
     stat(crp1, &st);
     chmod(crp2, st.st_mode);
     printf("%s \n %s",crp1,crp2);
    recursive_copy(crp1,crp2);
  }
 }
}
return 1;
}
