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

void my_jobs(char **para, int tot_para, int *bag_proc, char **bg_inp) {
//  printf("2");
  int flagr = 0;
  int flags = 0;
  int nflag=0;
  for (int i = 0; i < tot_para; i++) {
    if (strcmp(para[i], "-rs") == 0 || strcmp(para[i], "-sr") == 0) {
      flagr = 1;
      flags = 1;
      nflag=1;
    }
    if (strcmp(para[i], "-r") == 0) {

      flagr = 1;
      nflag=1;
    }
    if (strcmp(para[i], "-s") == 0) {
      flags = 1;
      nflag=1;

    }
  }
  if (nflag == 0) {
    flags = 1;
    flagr =1;
  }

//printf("3");
printjobs(bag_proc,bg_inp,flagr,flags);

}

void printjobs(int *bag_proc, char **bg_inp, int flagr, int flags) {
  //printf("4");
  char file_name[4096];
  char stat;
  for (int i = 0; i < 4096; i++) {
    if (bag_proc[i] > 0) {
      sprintf(file_name, "/proc/%d/stat", bag_proc[i]);
     //printf("%s\n",file_name);

  FILE *file = fopen(file_name, "r");
  if (!file) {
    // printf("sorry");
    printf("%s not found \n",file_name);
    perror("Error");
    return;
  }
  //char *temp[2];
  fscanf(file, "%*d %*s %c", &stat);
  //printf("stat : %c",stat);
int x = i+1;
  if (stat == 'T') {



    if (flags) {
      printf("[%d] Stopped %s [%d]\n", x ,bg_inp[i], bag_proc[i]);
    }
  } else {
    if (flagr) {
      printf("[%d] Running %s [%d]\n", x,bg_inp[i], bag_proc[i]);
    }
  }
  fclose(file);
  }


}}
