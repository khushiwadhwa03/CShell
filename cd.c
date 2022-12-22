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

int my_cd(char **para, char *home_d, char *prev_path) {

  int x;
  char output[4096];
  output[4095] = '\0';
  if (para[1] == NULL) { // if input is cd   //
    para[1] = home_d;
    strcpy(output, home_d);
  }
  if (para[1][0] == '~') {
    getcwd(prev_path, 4096);
    strcpy(output, home_d);
  } else if (para[1][0] == '-') {
    strcpy(output, prev_path);
    printf("%s\n", output);
    getcwd(prev_path, 4096);

  }

  else {
    strcpy(output, para[1]);
    getcwd(prev_path, 4096);
  }

  x = chdir(output);
  if (x < 0)
    perror("error");
  return 1;
}
