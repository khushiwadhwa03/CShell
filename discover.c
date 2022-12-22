#include "discover.h"
#include "cd.h"
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

void disco(char *name, int dflag, int fflag) {

  int flag_dir = 0;
  struct stat fs = {0};

  stat(name, &fs);
  if (S_ISDIR(fs.st_mode)) {
    flag_dir = 1;
  }
  if (flag_dir == 0) {
    if (fflag == 1) {
      if (name[0] != '.')
        printf("%s\n", name);
    }
  } else {
    if (dflag == 1)
      if (name[0] != '0')
        printf("%s\n", name);

    DIR *d;
    struct dirent *dir;
    d = opendir(name);
    if (d) {
      while ((dir = readdir(d)) != NULL) {
        if (strcmp(dir->d_name, ".") == 0 || strcmp(dir->d_name, "..") == 0) {
          continue;
        }
        char newname[4096];
        strcpy(newname, name);
        strcat(newname, "/");
        strcat(newname, dir->d_name);
        int flag_dir_ = 0;
        struct stat fs = {0};

        stat(newname, &fs);
        if (S_ISDIR(fs.st_mode))
          flag_dir_ = 1;
        if (flag_dir_ == 0) {
          if (fflag == 1) {
            printf("%s\n", newname);
          }
        } else {

          disco(newname, dflag, fflag);
        }
      }
    }
  }
}

void disc(char *name, char *fname, char *fixed) {

  int flag_dir = 0;
  struct stat fs = {0};

  stat(name, &fs);
  if (S_ISDIR(fs.st_mode)) {
    flag_dir = 1;
  }

  if (flag_dir == 0) {
    if (strcmp(name, fname) == 0) {

      printf("%s\n", name);
      return;
    }
  } else {

    DIR *d;
    struct dirent *dir;
    d = opendir(name);
    if (d) {
      while ((dir = readdir(d)) != NULL) {
        if (strcmp(dir->d_name, ".") == 0 || strcmp(dir->d_name, "..") == 0) {
          continue;
        }
        char newname[4096];
        strcpy(newname, name);
        strcat(newname, "/");
        strcat(newname, dir->d_name);
        char newfile[4096];

        strcpy(newfile, name);
        strcat(newfile, "/");
        strcat(newfile, fixed);

        int flag_dir_ = 0;
        struct stat fs = {0};

        stat(newname, &fs);
        if (S_ISDIR(fs.st_mode))
          flag_dir_ = 1;
        if (flag_dir_ == 0) {

          if (strcmp(fixed, dir->d_name) == 0) {
            printf("%s\n", newname);
            return;
          }
        } else {

          disc(newname, newfile, fixed);
        }
      }
    }
  }
}
