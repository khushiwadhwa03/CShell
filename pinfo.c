#include "pinfo.h"
#include "cd.h"
#include "echo.h"
#include "ls.h"
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

int my_pinfo(char *root, char *base) {
  char *fnames = (char *)malloc(4096);
  strcpy(fnames, "/proc/");
  strcat(fnames, base);
  strcat(fnames, "/stat");

  int pid, mem, pg, tp;
  char *fpath = (char *)malloc(4096);
  char sts;
  FILE *f = fopen(fnames, "r");
  if (f == 0) {
    perror("error");
    return 0;
  } else {
    char temp[30][4096];
    fscanf(f, "%d ", &pid);
    fscanf(f, "%s ", temp[1]);
    fscanf(f, "%c ", &sts);
    fscanf(f, "%s ", temp[2]);
    fscanf(f, "%d ", &pg);
    fscanf(f, "%s ", temp[3]);
    fscanf(f, "%s ", temp[4]);
    fscanf(f, "%d ", &tp);
    fscanf(f, "%s ", temp[5]);
    fscanf(f, "%s ", temp[6]);
    fscanf(f, "%s ", temp[7]);
    fscanf(f, "%s ", temp[8]);
    fscanf(f, "%s ", temp[9]);
    fscanf(f, "%s ", temp[10]);
    fscanf(f, "%s ", temp[11]);
    fscanf(f, "%s ", temp[12]);
    fscanf(f, "%s ", temp[13]);
    fscanf(f, "%s ", temp[14]);
    fscanf(f, "%s ", temp[15]);
    fscanf(f, "%s ", temp[16]);
    fscanf(f, "%s ", temp[17]);
    fscanf(f, "%s ", temp[18]);
    fscanf(f, "%d ", &mem);

    char d = '+';
    if (pg != tp) {
      d = ' ';
    }
    printf("pid -- %d\nProcess Status -- %c%c\nmemory-- %d\n", pid, sts, d,
           mem);
  }
  fclose(f);
  strcpy(fnames, "/proc/");
  strcat(fnames, base);
  strcat(fnames, "/exe");

  int len = readlink(fnames, fpath, 4095);
  if (len == 0) {
    perror("error");
  } else {
    fpath[len] = '\0';

    for (int i = 0; i < 4096; i++) {
      if (root[i] == '\0') {
        break;
      }
      if (i == len)
        break;
      if (fpath[i] != root[i])
        break;
    }
    printf("Executable path -- %s\n", fpath);
  }
  return 1;
}
