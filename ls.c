#include "ls.h"
#include "cd.h"
#include "echo.h"
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

void sort(char arr[][4096], int n) {
  char temp[4096];
  for (int i = 0; i < n - 1; i++) {
    for (int j = 0; j < n - 1 - i; j++) {
      if (strcmp(arr[j], arr[j + 1]) > 0) {
        strcpy(temp, arr[j]);
        strcpy(arr[j], arr[j + 1]);
        strcpy(arr[j + 1], temp);
      }
    }
  }
}

void lprint(char filename[], char f_name[]) {

  struct stat fs = {0};
  stat(filename, &fs);
  int exec = 0;
  int direc = 0;
  int norm = 0;

  // checking mode
  if (S_ISREG(fs.st_mode)) {
    norm = 1;
    printf("- ");
  } else if (S_ISDIR(fs.st_mode)) {
    direc = 1;
    printf("d ");
  } else if (S_IEXEC & fs.st_mode) {
    exec = 1;
    printf("l ");
  }
  // permissions
  if (fs.st_mode & S_IRUSR)
    printf("r");
  else
    printf("-");
  if (fs.st_mode & S_IWUSR)
    printf("w");
  else
    printf("-");
  if (fs.st_mode & S_IXUSR)
    printf("x");
  else
    printf("-");
  if (fs.st_mode & S_IRGRP)
    printf("r");
  else
    printf("-");
  if (fs.st_mode & S_IWGRP)
    printf("w");
  else
    printf("-");
  if (fs.st_mode & S_IXGRP)
    printf("x");
  else
    printf("-");
  if (fs.st_mode & S_IROTH)
    printf("r");
  else
    printf("-");
  if (fs.st_mode & S_IWOTH)
    printf("w");
  else
    printf("-");
  if (fs.st_mode & S_IXOTH)
    printf("x ");
  else
    printf("- ");
  struct passwd *pws;
  struct group *grp;
  char fulltime[1000];
  fulltime[999] = '\0';
  strftime(fulltime, 1000, "%b %d %H:%M", localtime(&fs.st_mtime));
  grp = getgrgid(fs.st_gid);
  pws = getpwuid(fs.st_uid);
  int temp = (int)fs.st_nlink;
  int temp2 = (int)fs.st_size;
  printf("%d %s %s %d %s ", temp, pws->pw_name, grp->gr_name, temp2, fulltime);
  if (direc == 1) {
    printf("\033[0;34m");
  } else if (exec == 1) {
    printf("\033[0;32m");
  } else {
    printf("\033[0;37m");
  }
  printf("%s\n", f_name);
  printf("\033[0m");
  return;
}

void normal(char *name, int aflag, int flagl) {

  char norm[1000][4096];
  int ind = 0;

  int isdir = 0;
  if (opendir(name)) {
    char copy[4096];
    copy[4095] = '\0';
    DIR *d;
    struct dirent *dir;
    int x = strlen(name);

    if (name[x - 1] != '/') {

      char newname[4096];
      strcpy(newname, name);
      strcat(newname, "/");
      newname[4095] = '\0';
      name = newname;
      closedir(d);
    }
    strcpy(copy, name);
    d = opendir(name);
    if (d) {
      isdir = 1;
      if (flagl == 0) {
        while ((dir = readdir(d)) != NULL) {
          int flag_ = 0;
          if (dir->d_name[0] == '.' && aflag == 1) {
            flag_ = 1;

          } else if (dir->d_name[0] != '.') {
            flag_ = 1;
          }
          if (flag_ == 1) {
            strcpy(norm[ind], dir->d_name);

            ind++;
          }
        }

        closedir(d);
      } else if (flagl == 1) {
        while ((dir = readdir(d)) != NULL) {
          if (dir->d_name[0] == '.' && aflag == 1) {
            strcat(copy, dir->d_name);
            lprint(copy, dir->d_name);
            strcpy(copy, name);
          } else if ((dir->d_name[0] != '.')) {
            strcat(copy, dir->d_name);
            lprint(copy, dir->d_name);
            strcpy(copy, name);
          }
        }
      }
    }
  } else {
    if (flagl)
      lprint(name, name);
    else {
      strcpy(norm[ind], name);

      ind++;
    }
  }
  if (flagl == 0) {
    sort(norm, ind);
    for (int i = 0; i < ind; i++) {
      struct stat fs = {0};
      stat(norm[i], &fs);
      int exec = 0;
      int direc = 0;
      int normal = 0;

      if (S_ISREG(fs.st_mode)) {
        normal = 1;

      } else if (S_ISDIR(fs.st_mode)) {
        direc = 1;

      } else if (S_IEXEC & fs.st_mode) {
        exec = 1;
      }
      if (direc == 1) {
        printf("\033[0;34m");
      } else if (exec == 1) {
        printf("\033[0;32m");
      } else {
        printf("\033[0;37m");
      }
      printf("%s\n", norm[i]);
      printf("\033[0m");
    }
  }

  return;
}

void my_ls(char **para, char home_dir[], int tot_para) {
  int flaga = 0;
  int flagl = 0;
  int flag = 0;

  for (int i = 0; i < tot_para; i++) {
    if (strcmp(para[i], "-al") == 0 || strcmp(para[i], "-la") == 0) {
      flagl = 1;
      flaga = 1;
    }
    if (strcmp(para[i], "-a") == 0) {

      flaga = 1;
    }
    if (strcmp(para[i], "-l") == 0) {
      flagl = 1;
    }
  }

  for (int i = 0; i < tot_para; i++) {
    if (strcmp(para[i], "ls") != 0 && strcmp(para[i], "-a") != 0 &&
        strcmp(para[i], "-l") != 0 && strcmp(para[i], "-al") != 0 &&
        strcmp(para[i], "-la") != 0) {
      char act_path[PATH_MAX] = {'\0'};

      strcpy(act_path, home_dir);
      if (para[i][0] == '~')
        strcat(act_path, para[i] + 1);
      else
        strcpy(act_path, para[i]);
      flag = 1;
      normal(act_path, flaga, flagl);
    }
  }
  // if no dir name given , perform ls in current directory
  if (flag == 0)
    normal(".", flaga, flagl);
  return;
}
