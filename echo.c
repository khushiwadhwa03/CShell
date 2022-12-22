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

void my_echo(long long int tot_para, char *para[]) {
  for (int a = 1; a < tot_para - 1; a++) {
    printf("%s ", para[a]);
  }
  printf("\n");
}
