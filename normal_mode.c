//Manojit Chakraborty
//2018201032
#include <unistd.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <time.h>
#include <stdlib.h>
#include <termios.h>
#include <pwd.h>
#include <grp.h>
#include <errno.h>
#include <string.h>
#include <sys/ioctl.h>
#include "command_mode.h"
#include "normal_help.h"

int search_func(char* command);
void recursive_search_func(char chz1[],FILE* fp,char dirname[]);
void normal_mode(char *prev_visited);
FILE* fp;
char main_crr[FILENAME_MAX];
int cursor_row,cursor_column, cursor_x=1,cursor_y=1, c=1, l=1,flag=0;
char root_crr[FILENAME_MAX], root_crr1[FILENAME_MAX], front_stack[300][300], back_stack[300][300], chz[1000], command1[1000];
int top=-1, fsTop1 = -1;



int command_mode()
{
  flag=1;
  gotoxy(cursor_row-8,cursor_column-8);
  printf("\033[1m\033[36;1m COMMAND MODE \033[0m");
  printf("\033[%d;1H",cursor_row);
  char cmd;
  cmd = getch();
  if((int)cmd==27){
    printf("\033[1;1H");
    flag=0;
  }
  else{
    putchar(cmd);
    char command[100];
    command[0]=cmd;
    int index =1;
  do{
    char cmd1;
    cmd1 = getch();
    if((int)cmd1==27){
      printf("\033[1;1H");
      flag=0;
      break;
    }
    else if((int)cmd1==127){
      printf("\033[1D");
      printf(" ");
      printf("\033[1D");
      index--;
    }
    else if(cmd1=='\n'){
      command[index]='\0';
      if(command[0]=='s'&&command[1]=='e'&&command[4]=='c'&&command[5]=='h'){
          int i;
          top=top+1;
          for(i=0;i<=strlen(chz);i++)
          {
            front_stack[top][i]=chz[i];
          }
        search_func(command);
      }
      else if(command[0]=='g'&&command[1]=='o'&&command[2]=='t'&&command[3]=='o'){
        int i=0,j=0;
        char dirname[100];
        for(i=0;i<strlen(command);i++){
          if(command[i]==' '){
            break;
          }
        }
        for(j=i+1;j<=strlen(command);j++){
          dirname[j-i-1]=command[j];
        }
        if(dirname[0]=='/'){
           char new_crr[FILENAME_MAX];
              int i = 0;
              for(i=0;i<=strlen(root_crr);i++){
                 new_crr[i]=root_crr[i];}
              normal_mode(new_crr);
	         continue;
        }
        strcat(chz,"/");
        strcat(chz,dirname);
        if(dirname[0]=='.'&&dirname[1]=='.')
        {
          int i3;
          top=top+1;
          for(i3=0;i3<=strlen(chz);i3++)
          {
            front_stack[top][i3]=chz[i3];
          }
        }
        normal_mode(chz);
      }
      else if(strcmp(command,"exit")==0){
        printf("\n");
        exit(0);
      }
      else{
      start_loop(command,chz);
      normal_mode(chz);
    }
    }
    else{
    putchar(cmd1);
    command[index++]=cmd1;
   }
  }while(1);
 }
}


void command_loop(char *prev_visited,char dirlist[1000][300],char dirname[],int t1)
{
  l = 1;
  cursor_x = 1;
  do{
    c = t1;
    struct winsize size;
    ioctl(STDOUT_FILENO,TIOCGWINSZ,&size);
    cursor_row=size.ws_row-5;
    cursor_column=size.ws_col/2;
    terminal_refresh(dirlist,cursor_row,cursor_column,l,flag);
    if(flag==1)
    command_mode();
    else
    {
    gotoxy(cursor_x,cursor_y);
    char ch;
    ch = getch();
          if((int)ch==10)
          {
              char filedir[FILENAME_MAX];
              char filedir1[FILENAME_MAX];
              getcwd(filedir1,sizeof(filedir));
              strcpy(filedir,prev_visited);
              strcat(filedir,"/");
              strcat(filedir,dirlist[l-1]);
              if (isFile(dirlist[l-1])==1 ){
                chdir(prev_visited);
                if(!fork())
                execlp("xdg-open","xdg-open",dirlist[l-1],NULL);
                chdir(filedir1);
                continue;
              }
            else{
            normal_mode(dirlist[l-1]);}
          }
          if((int)ch==65){
            if(c<10){
              if(cursor_x>1){
              cursor_x=cursor_x-1;
              l=l-1;}
            }
            else if(l>1&&cursor_x>1)
            {
              if(l<11){
                cursor_x=cursor_x-1;
              }
              l=l-1;
            }
            else if(l>1&&cursor_x==1)
              l=l-1;
            gotoxy(1,1);
          }
          if((int)ch==66){
            if(c<10){
              if(cursor_x<c&&l<c){
                cursor_x+=1;
                l=l+1;}
              }
            else if(l<c&&cursor_x<10)
            {l=l+1;cursor_x=cursor_x+1;}
            else if(l<c&&cursor_x==10)
            l=l+1;
            gotoxy(1,1);
          }
          if((int)ch==127)
          {
              normal_mode(prev_visited);
          }
          if(ch==':'){
          command_mode();
          }
        }
  }while(1);
}

int search_func(char* command){
  int i=0,j=0;
  for(j=0;j<=strlen(command);j++){
    command1[j]=command[j];
  }
  char dirname[100];
  for(i=0;i<strlen(command);i++){
    if(command[i]==' '){
      break;
    }
  }
  for(j=0;j<=strlen(root_crr);j++){
    root_crr1[j]=root_crr[j];
  }
  strcat(root_crr1,"/");
  strcat(root_crr1,"dir1.txt");
  for(j=i+1;j<=strlen(command);j++){
    dirname[j-i-1]=command[j];
  }
  FILE* fp3 = fopen("dir1.txt","w+");
  recursive_search_func(chz,fp3,dirname);
  int t1=0;
  char dirlist[1000][300];
  while(fgets(dirlist[t1],300, fp3)) {
      dirlist[t1][strlen(dirlist[t1]) - 1] = '\0';
      t1++;
  }
  flag=0;
  command_loop(chz,dirlist,dirname,t1);
}


void recursive_search_func(char chz1[],FILE* fp,char dirname[]){
  struct dirent *dp;
  DIR *dir = opendir(chz1);

  while ((dp = readdir(dir)) != NULL)
  {
      if (strcmp(dp->d_name, ".") != 0 && strcmp(dp->d_name, "..") != 0)
      {
          freopen(root_crr1,"a+",fp);
          char folder[FILENAME_MAX];
          char folder1[FILENAME_MAX];
          int i2;
          for(i2=0;i2<=strlen(chz1);i2++)
              folder1[i2]=chz1[i2];
          for(i2=0;i2<=strlen(chz1);i2++)
              folder[i2]=chz1[i2];
          if(strcmp(dirname,dp->d_name)==0){
            strcat(folder, "/");
            strcat(folder, dp->d_name);
            fprintf(fp,"%s\n",folder);
          }
            strcat(folder1,"/");
            strcat(folder1,dp->d_name);
            if(isFile(folder1)!=1)
            recursive_search_func(folder1,fp,dirname);
      }
  }
}

void normal_mode(char *prev_visited){
    int i2;
    char dirlist[1000][300];
    for(i2=0;i2<=strlen(prev_visited);i2++)
      chz[i2]=prev_visited[i2];
    DIR *dp;
    FILE* fp =  fopen("dir.txt","w+");
    cursor_x=1,cursor_y=1;
    c=1;l=1;
    struct dirent *dir_ptr;
    char *a[1000];
    printf("\033[?1049h\033[H");
    dp = opendir(prev_visited);
    while ((dir_ptr = readdir(dp)) != NULL){
        struct stat fl_st;
        stat(dir_ptr->d_name,&fl_st);
        char cz[FILENAME_MAX];
        int i=0;
        for(i=0;i<=strlen(prev_visited);i++)
            cz[i]=prev_visited[i];
        strcat(cz,"/");
        if(strcmp(dir_ptr->d_name,"dir.txt")==0)continue;
 	      fprintf(fp, (opendir(strcat(cz,dir_ptr->d_name))) ? " d" : " -");
        fprintf(fp, (fl_st.st_mode & S_IRUSR) ? "r" : "-");
        fprintf(fp, (fl_st.st_mode & S_IWUSR) ? "w" : "-");
        fprintf(fp, (fl_st.st_mode & S_IXUSR) ? "x" : "-");
        fprintf(fp, (fl_st.st_mode & S_IRGRP) ? "r" : "-");
        fprintf(fp, (fl_st.st_mode & S_IWGRP) ? "w" : "-");
        fprintf(fp, (fl_st.st_mode & S_IXGRP) ? "x" : "-");
        fprintf(fp, (fl_st.st_mode & S_IROTH) ? "r" : "-");
        fprintf(fp, (fl_st.st_mode & S_IWOTH) ? "w" : "-");
        fprintf(fp, (fl_st.st_mode & S_IXOTH) ? "x" : "-");
	fprintf(fp,"  ");
	fprintf(fp,"%ld  ",fl_st.st_nlink);
	fprintf(fp,"%s  ",(getpwuid(fl_st.st_uid))->pw_name);
	fprintf(fp,"%s  ",(getgrgid(fl_st.st_gid))->gr_name);
  fprintf(fp,"%ld  ",fl_st.st_size);
	char date[10];
	strftime(date, 20, "%d-%m-%y", localtime(&(fl_st.st_ctime)));
	fprintf(fp,"%s  ",date);
        fprintf(fp,"%s\n",dir_ptr->d_name);
        a[c++]=dir_ptr->d_name;
    }
    c--;
    fclose(fp);
    int t=0;
    FILE *dir_fp = NULL;
    dir_fp = fopen("dir.txt", "r");
    while(fgets(dirlist[t],300, dir_fp)) {
        dirlist[t][strlen(dirlist[t]) - 1] = '\0';
        t++;
    }
    fclose(dir_fp);
    int i;
    do{
      struct winsize size;
      ioctl(STDOUT_FILENO,TIOCGWINSZ,&size);
      cursor_row=size.ws_row-5;
      cursor_column=size.ws_col/2;
      terminal_refresh(dirlist,cursor_row,cursor_column,l,flag);
      if(flag==1)
      command_mode();
      else
      {
      gotoxy(cursor_x,cursor_y);
      char ch;
      ch = getch();
            if((int)ch==10)
            {
              char filedir[FILENAME_MAX];
              char filedir1[FILENAME_MAX];
              getcwd(filedir1,sizeof(filedir));
              strcpy(filedir,prev_visited);
              strcat(filedir,"/");
              strcat(filedir,a[l]);
              if (isFile(filedir)==1 ){
                chdir(prev_visited);
                if(!fork())
                execlp("xdg-open","xdg-open",a[l],NULL);
                chdir(filedir1);
                continue;
              }
              else{
                if((a[l][0]=='.') && (a[l][1]=='.')){
                  if(strcmp(prev_visited,root_crr)!=0){
                    int i;
                    for(i=strlen(prev_visited)-1;i>0;i--){
                    if(prev_visited[i]=='/'){
                     prev_visited[i]='\0';
                     break;
                    }
                  }
                }
              }
              else if((a[l][0]=='.')){}
              else{
                strcat(prev_visited,"/");
                strcat(prev_visited,a[l]);
              }
              top=top+1;
              for(i=0;i<=strlen(prev_visited);i++)
              {
                front_stack[top][i]=prev_visited[i];
              }
              fsTop1=-1;
              normal_mode(prev_visited);
            }
            }
            if((int)ch==65){
              if(c<10){
              if(cursor_x>1&&l>1)
              {
              cursor_x=cursor_x-1;
              l=l-1;
            }
              }
              else if(l>1&&cursor_x>1)
              {if(l<11)cursor_x=cursor_x-1;l=l-1;}
              else if(l>1&&cursor_x==1)
              l=l-1;
              gotoxy(1,1);
            }
            if((int)ch==66){
              if(c<10){
                if(cursor_x<c&&l<c){
                  cursor_x+=1;
                  l=l+1;}
                }
              else if(l<c&&cursor_x<10)
              {l=l+1;cursor_x=cursor_x+1;}
              else if(l<c&&cursor_x==10)
              l=l+1;
              gotoxy(1,1);
            }
            if((int)ch==67){
                if(fsTop1!=-1)
                {
              top=top+1;
              for(i=0;i<=strlen(back_stack[fsTop1]);i++)
              {
                front_stack[top][i]=back_stack[fsTop1][i];
              }fsTop1--;
              normal_mode(front_stack[top]);
            }
            }
            if((int)ch==68)
            {
              if(strcmp(prev_visited,root_crr)!=0)
              {
              if(top!=0)
              {
             fsTop1=fsTop1+1;
              for(i=0;i<=strlen(front_stack[top]);i++)
              {
                back_stack[fsTop1][i]=front_stack[top][i];
              }top=top-1;
              normal_mode(front_stack[top]);
            }
            }
          }
            if((int)ch==127)
            {
              if(strcmp(prev_visited,root_crr)!=0)
              {
                  int i;
                  fsTop1=fsTop1+1;
                  for(i=0;i<=strlen(front_stack[top]);i++)
                  {
                    back_stack[fsTop1][i]=front_stack[top][i];
                  }top--;
                  for(i=strlen(prev_visited)-2;i>0;i--){
                      if(prev_visited[i]=='/'){
                             prev_visited[i]='\0';
                             break;
                      }
                   }
                  normal_mode(prev_visited);
              }
            }
            if(ch==':'){
            command_mode();
            }
            if(ch=='h'){
              char new_crr[FILENAME_MAX];
              int i = 0;
              for(i=0;i<=strlen(root_crr);i++){
                 new_crr[i]=root_crr[i];}
              normal_mode(new_crr);
            }
          }
    }while(1);
}


int main()
{
    int i;
    printf("\033[?1049h\033[H");
    getcwd(main_crr, sizeof(main_crr));
    for(i=0;i<=strlen(main_crr);i++)
        root_crr[i]=main_crr[i];
    top=top+1;
    for(i=0;i<=strlen(root_crr);i++)
    {
      front_stack[top][i]=root_crr[i];
    }
    normal_mode(main_crr);
    return EXIT_SUCCESS;
}
