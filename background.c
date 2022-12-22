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

void bg(int *bag_proc, char **bg_inp, char **para) {

  int pid = fork();
  if (pid < 0) {
    perror("Error");
  } else if (!pid) {
    setpgid(0, 0);

    if (execvp(para[0], para) < 0) {
      perror("Error");
      exit(1);
    }
  } else {

    printf("%s started : PID [%d]\n", para[0], pid);

    for (int i = 0; i < 4096; i++) {
      if (bag_proc[i] == 0) {
        bag_proc[i] = pid;
        int x = strlen(para[0]);
        x++;
        bg_inp[i] = malloc(x);
        strcpy(bg_inp[i], para[0]);
        break;
      }
    }
  }
}
