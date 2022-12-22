#include "background.h"
#include "cd.h"
#include "discover.h"
#include "echo.h"
#include "foreground.h"
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
#include "jobs.h"
#include "sig.h"

void my_sig(char **para, int tot_para, int *bag_proc, char **bg_inp) {
  if (tot_para < 3) {
    printf("Too few arguments\n");
    return ;
  }
  else if (tot_para > 3) {
    printf("Too many arguments\n");
    return ;
  }
  else{
    int no = atoi(para[1]);
    if (no <= 0 || no > 4096) {
      printf("invalid job number \n");
      return;
    }
  int signo = atoi(para[2]);
  int pid = bag_proc[no - 1];

  if (pid <= 0) {
    printf("Invalid job number\n");
    return;
  }
  if (kill(pid, signo) < 0)
    {
        perror("sig");
        return;
  } else {
    //printf ("successfully done");
  }
  if (signo == 9) {
    fprintf(stderr, "%s with pid [%d] exited abnormally\n", bg_inp[no - 1],
            bag_proc[no - 1]);
    bag_proc[no-1]=0;
  }
}
}
