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


void gotoxy(int x, int y) {
    printf("\033[%d;%dH",x,y);
}

int terminal_refresh(char line[1000][300],int cursor_row,int cursor_column,int l,int flag)
{
  FILE* fp1 =  fopen("dir.txt","r");
  int line_number = 1, i,cursor1,cursor2;
  if(l>10){
    cursor1 = l-9;
    cursor2 = l;
  }
  else{
    cursor1 = 1;
    cursor2 = 10;
  }
  printf("\033[?1049h\033[H");
   for(i=cursor1-1;i<cursor2;i++)
   {
     printf("%s\n",line[i]);
   }
   if(flag==0)
   {
     gotoxy(cursor_row-8,cursor_column-7);
     printf("\033[1m\033[32;1m NORMAL MODE \033[0m");

   }
   gotoxy(1,1);
   return 0;
}


int getch(void)
{
    struct termios oldattr, newattr;
    int ch;
    tcgetattr( STDIN_FILENO, &oldattr );
    newattr = oldattr;
    newattr.c_lflag &= ~( ICANON | ECHO );
    tcsetattr( STDIN_FILENO, TCSANOW, &newattr );
    ch = getchar();
    tcsetattr( STDIN_FILENO, TCSANOW, &oldattr );
    return ch;
}



int isFile(const char* name)
{
    DIR* directory = opendir(name);
    if(directory != NULL)
    {
     closedir(directory);
     return 0;
    }
    if(errno == ENOTDIR)
    {
     return 1;
    }
    return -1;
}
