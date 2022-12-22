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
#include "fg.h"

#include "bg.h"

void my_backg(char **para, int tot_para, int *bag_proc, char **bg_inp) {
  if (tot_para != 2) {
    printf("Error : Invalid number of arguments");
    return;
  }

  int jobno = atoi(para[1]);
  if (jobno <= 0 || jobno > 4096) {
    printf("Error : Invalid job number\n");
    return;
  }
  int job_pid = bag_proc[jobno - 1];
 // printf("%d %d\n", jobno, job_pid);
  if (kill(job_pid, SIGCONT) < 0) {
    perror("Error");
    return ;
  }
  }
