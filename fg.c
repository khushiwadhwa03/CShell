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

void my_foreg(char **para, int tot_para, int *bag_proc, char **bg_inp) {
  int status;
  if (tot_para != 2) {
    printf("Error");
  }
  int jobno = atoi(para[1]);
  if (jobno <= 0 || jobno > 4096) {
    printf("Error : Invalid job number\n");
    return;
  }
  int job_pid = bag_proc[jobno - 1];

  setpgid(job_pid, getpgid(0));
  signal(SIGTTOU, SIG_IGN);
  signal(SIGTTIN, SIG_IGN);
  tcsetpgrp(0, job_pid);
  if (kill(job_pid, SIGCONT) < 0)
    {
        perror("fg");
        return;
  }
    waitpid(job_pid, &status, WUNTRACED);
    tcsetpgrp(0, getpgid(0));

    fprintf(stderr, "%s with pid [%d] exited abnormally\n", bg_inp[jobno - 1], bag_proc[jobno - 1]);

    signal(SIGTTOU, SIG_DFL);
    signal(SIGTTIN, SIG_DFL);
    bag_proc[jobno-1] = 0;
}
