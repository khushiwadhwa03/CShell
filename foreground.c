#include "foreground.h"
#include "cd.h"
#include "discover.h"
#include "echo.h"
#include "ls.h"
#include "pinfo.h"
#include "pwd.h"
#include <ctype.h>
#include <dirent.h>
#include <fcntl.h>
#include <grp.h>
#include <pwd.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>
int pid;

char * fgname;
int fg(char *words[]) {
 // printf("ll%d\n",getpid());
  pid = fork();
  fgpid = pid;
//   printf("mm%d\n",fg_pid);
   fgname=words[0];
  if (pid < 0) {
    printf("ERROR");
    return -1;
  } else if (pid != 0) {

    int stat;

    time_t x = time(NULL);
    waitpid(pid, &stat, WUNTRACED);
    time_t y = time(NULL);
   // fgpid = pid;
   // printf("%d",fg_pid);
    return ((int)(y - x));


  } else {

    if (execvp(words[0], words) < 0) {
      printf("ERROR");
      exit(1);
    }
    return (-1);
  }
}
