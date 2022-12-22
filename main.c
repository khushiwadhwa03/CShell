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
#include <signal.h>
int fgpid;
char host[4096];
char home_d[4096];
char sys_name[4096];
char curr_path[4096];
char prev_path[4096];
char curr_d[4096];
char hist_path[4096];
char *user_name;
int count_hist;
int tim;
int saved;
int saved_inp;
int file_desc;
int inp_file_desc;
int bag_proc[4096];
char *bg_inp[4096];
int is_bg;
int st;
int pipe_flag;
int shell_pid;
extern int pid;
extern char *fgname;

void add_tilde(char cdir[], char path[], char curr_path[], char home_d[]) {
  int len_cp = strlen(curr_path);
  int len_hd = strlen(home_d);
  cdir[0] = '~';
  int flag = 0;

  if (len_cp >= len_hd) {
    flag = 1;

    if (strncmp(curr_path, home_d, len_hd) != 0)
      flag = 0;

    else {
      int x = 1;
      for (int i = len_hd; i < len_cp; i++) {
        cdir[x] = curr_path[i];
        x++;
      }
      cdir[x] = '\0';
    }
  }

  if (flag == 0)
    strcpy(cdir, curr_path);
}
void c_handler() { return; }
void z_handler() {



  for (int i = 0; i < 4096; i++) {
    if (bag_proc[i] == 0) {
      bag_proc[i] = fgpid;
      bg_inp[i] = fgname;
      break;
    }
  }

  kill(fgpid,SIGTSTP);

  return;


}

int main() {
  fgpid =0;
  shell_pid = getpid();
  getcwd(home_d, 4096);
  getcwd(prev_path, 4096);
  getcwd(hist_path, 4096);
  // starting infinite loop
  signal(SIGTSTP, z_handler);
  // signal(SIGCHLD, cntrlbg);
  signal(SIGINT, c_handler);




  char array[21][4096];
  long unsigned size = 1024;
  strcpy(hist_path, home_d);
  strcat(hist_path, "/history.txt");
  // reading history from file into an array , which will be updated every time
  // a command is entered
  FILE *hfile;
  hfile = fopen(hist_path, "r");
  count_hist = 0;
  if (!hfile) {
    fopen(hist_path,
          "w"); // if the file doesnt already exist , we create a file
  } else {

    char *command = (char *)malloc(4096);
    char buff[4096];
    int d = 0;
    while (fgets(buff, 4096, hfile) != NULL) {

      buff[strlen(buff) - 1] = '\0';
      strcpy(array[d], buff);
      d++;
      count_hist++;
    }
  }
  fclose(hfile);

  tim = -1;
  while (1) {

    gethostname(host, 4096);

    uid_t uid = geteuid(); // gets effective user id
    struct passwd *sys_uid =
        getpwuid(uid); // part of pwd.h header , has user info
    user_name = sys_uid->pw_name;
    getcwd(curr_path, 4096);
    gethostname(sys_name, 4096);
    add_tilde(curr_d, curr_path, curr_path, home_d);

    for (int i = 0; i < 4096; i++) {
      if (bag_proc[i] > 0) {
        if (waitpid(bag_proc[i], &st, WNOHANG) >= 1) {

          if (WIFEXITED(st)) {
            fprintf(stderr, "%s with pid [%d] exited normally\n", bg_inp[i],
                    bag_proc[i]);
          } else {
            fprintf(stderr, "%s with pid [%d] exited abnormally\n", bg_inp[i],
                    bag_proc[i]);
          }
          bag_proc[i] = 0;
        }
      }
    }
    if (tim < 1) {
      printf("\033[1;33m");
      printf("<%s@%s:", user_name, sys_name);
      printf("\033[1;36m");
      printf("%s>", curr_d);
      printf("\033[0m");

    } else { // if last command was a forground command
      printf("\033[1;33m");
      printf("<%s@%s:", user_name, sys_name);
      printf("\033[1;36m");
      printf("%s>", curr_d);
      printf("\033[0m");
      printf("took %ds >", tim);
    }
    tim = -1;
    // prompt complete

    char *input = NULL;
    size_t buffers = 4096;
    size_t xo;
    xo = getline(&input, &buffers, stdin);
    if (xo == -1) {
      return 0;
    }
    if (input == NULL) {
      return 0;
    }
    // if (input == -1) {
    //   exit(0);
    // }
    if (strcmp(input, "\n") == 0) {
      continue;
    }
    if (input == NULL) {
      return -1;
    }
    char temp[4096];
    strcpy(temp, input);
    temp[strcspn(temp, "\n")] = '\0'; // to remove the extra '\n'
    // printf("input : %s\n", input);
    // printf("hist : %s \n",temp);
    if (count_hist < 20) // if no of history commands stored <20 , add the
                         // command to the array
    {
      if (strcmp(array[count_hist - 1], temp) != 0) {
        strcpy(array[count_hist], temp);
        count_hist++;
      }
    } else // else delete the oldest command and add the latest
    {
      if (strcmp(array[count_hist - 1], temp) != 0) {
        for (int i = 0; i < 19; i++) {
          strcpy(array[i], array[i + 1]);
        }
        strcpy(array[19], temp);
      }
    }

    char *outermost[1000];
    char *outer = strtok(input, ";"); // tokenise based on semicolons
    int tot_comm = 0;
    if (outer != NULL) {
      outermost[0] = outer;
      int o = 1;
      while (1) {

        if (outer == NULL) {
          break;
        }
        outer = strtok(NULL, ";");
        outermost[o] = outer;
        o++;
      }
      tot_comm = o;
    }
    for (int o = 0; o < tot_comm - 1; o++) {
      int no_amp = 0;
      for (int q = 0; q < strlen(outermost[o]); q++) {
        if (outermost[o][q] == '&') {
          no_amp++;
        }
      }
      //  printf("NO OF AMP = %d\n", no_amp);
      char *middlelayer[1000];
      char *middle = strtok(outermost[o], "&");
      int tot_parts = 0;
      if (middle != NULL) {
        middlelayer[0] = middle;
        int m = 1;
        while (1) {
          if (middle == NULL) {
            break;
          }
          middle = strtok(NULL, "&");
          middlelayer[m] = middle;
          m++;
        }
        tot_parts = m;
       // printf("total no of mid parts is %d\n",tot_parts);

      }
      for (int m = 0; m < tot_parts - 1; m++) {
        //check
        if (m < no_amp) {
          is_bg = 1;
        } else {
          is_bg = 0;
        }
        char *pipedlayer[1000];
        char *pipep = strtok(middlelayer[m], "|");
        int tot_pipes = 0;
        if (pipep != NULL) {
        pipedlayer[0] = pipep;
        int pi = 1;
        while (1) {
          if (pipep == NULL) {
            break;
          }
          pipep = strtok(NULL, "|");
          pipedlayer[pi] = pipep;
          pi++;
        }
        tot_pipes = pi;
        pipe_flag = 0;
        // printf("commands : \n");
        // for (int ss = 0; ss < tot_pipes - 1; ss++) {
        //   printf("h %s\n",pipedlayer[ss]);
        // }
        // printf("%d\n",tot_pipes);
        if (tot_pipes > 2) {
          pipe_flag=1;
        }
        }
       // printf("piped flag is %d",pipe_flag);
        int shell_in = dup(STDOUT_FILENO);
        int shell_out = dup(STDIN_FILENO);
        int tube[2];
        tube[0]=shell_in;
        if (pipe_flag == 1) {

          pipe(tube);
          }
        for (int po = 0; po < tot_pipes - 1; po++) {

        if (pipe_flag == 1) {

         //tube[0] = 0;
         //tube[1]=0;
        dup2(tube[1],STDOUT_FILENO);
        close(tube[1]);
       // if(po != 0 )
        dup2(tube[0], STDIN_FILENO);
        }
        int isredirectedout=0;
        int isredirectedin=0;
       // printf("starting the para part\n");
       // printf("command hai %s\n",pipedlayer[po]);
        char *para[1000];

        char *word = strtok(pipedlayer[po],
                            " \t\r\n\a"); // tokenise based on spaces tabs ,
                                          // newlines etc - separates words.

        int tot_para=0;
        if (word != NULL) {

          para[0] = word;
          int u = 1;
          while (1) {
            if (word == NULL) {
              break;
            }
            word = strtok(NULL, " \t\r\n\a");
            para[u] = word;
            u++;
          }
          tot_para = u;}
        //  printf("total no of para is %d\n",tot_para);
          char* outputfile;

          //REDIRECTION WALLA PART
          //int no_to_sub=0;;
          int i= tot_para-2;
          while(i>0){
            if (strcmp(para[i-1],">")==0){
                isredirectedout=1;
                outputfile = para[i];
               // printf("output in %s\n",outputfile);
                tot_para=tot_para-2;
                i=i-2;
                saved=dup(STDOUT_FILENO);
                file_desc = open(outputfile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
                dup2(file_desc, STDOUT_FILENO) ;

                }
            else if ((strcmp(para[i-1],">>")==0)){
                isredirectedout=2;
                outputfile = para[i];
               // printf("output in %s\n",outputfile);
                tot_para=tot_para-2;
                i=i-2;
                saved=dup(STDOUT_FILENO);
                file_desc = open(outputfile, O_WRONLY | O_APPEND);
                if (file_desc<1) {
                  file_desc = open(outputfile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
                }
                dup2(file_desc, STDOUT_FILENO) ;}
            else if (strcmp(para[i-1],"<")==0){
                isredirectedin=1;
                char* inputredfile = para[i];
                //printf("input in %s\n",inputredfile);
                tot_para=tot_para-2;
                i=i-2;
                saved_inp=dup(STDIN_FILENO);
                inp_file_desc = open(inputredfile, O_RDONLY);
                 if (!inp_file_desc) {
                  perror("Error");
                  continue;
                }
                dup2(inp_file_desc, STDIN_FILENO);
                }
            else{
              i=i-1;
            }



            }
           // printf("tot_para is now %d\n",tot_para);


          if (strcmp(para[0], "pwd") == 0) {
            my_pwd();
          } else if (strcmp(para[0], "echo") == 0) {
            my_echo(tot_para, para);
          } else if (strcmp(para[0], "cd") == 0) {
            if (tot_para > 3) {
              printf("too many parameters\n");
              continue;
            }
            my_cd(para, home_d, prev_path);
          } else if (strcmp(para[0], "ls") == 0) {

            my_ls(para, home_d, tot_para - 1);
          } else if ((strcmp(para[0], "pinfo") == 0)) {

            if (tot_para > 3) {
              printf("too many parameters");
              continue;
            } else if (tot_para == 3) {
              int x = my_pinfo(home_d, para[1]);
            } else {

              int x = my_pinfo(home_d, "self");
            }
          } else if (strcmp(para[0], "history") == 0) {

            int till;
            if (tot_para > 3) {
              till = 10;
              printf("too many parameters");
              continue;
            } else if (tot_para == 3) {
              till = atoi(para[1]);

              if (till > 20) {
                till = 10;
              }
            } else {
              till = 10;
            }
            if (count_hist < till) {

              till = count_hist;
            }

            for (int i = count_hist - 1; i >= count_hist - till; i--) {
              printf("%s\n", array[i]);
            }
          } else if ((strcmp(para[0], "discover") == 0)) {
            int fflag = 0;
            int dflag = 0;
            int mode = 0;

            for (int i = 0; i < tot_para - 1; i++) {
              if (strcmp(para[i], "-fd") == 0 || strcmp(para[i], "-df") == 0) {
                dflag = 1;
                dflag = 1;
              }
              if (strcmp(para[i], "-f") == 0) {

                fflag = 1;
              }
              if (strcmp(para[i], "-d") == 0) {
                dflag = 1;
              }
            }
            char *directory = ".";
            char file_n[4096];

            int flag_dir = 0;
            int flag_file = 0;

            for (int i = 0; i < tot_para - 1; i++) {
              if (strcmp(para[i], "-d") && strcmp(para[i], "-f") &&
                  strcmp(para[i], "discover")) {

                if (para[i][0] == '"') {
                  flag_file = 1;
                  mode = 1;
                  int x = strlen(para[i]);
                  for (int j = 1; j <= x - 2; j++) {
                    file_n[j - 1] = para[i][j];
                  }
                  file_n[x - 2] = '\0';
                } else {
                  flag_dir = 1;
                  directory = para[i];
                }
              }
            }

            if (dflag == 0 && fflag == 0) {
              dflag = 1;
              fflag = 1;
            }
            char current[4096];
            getcwd(current, 4096);

            char direc[4096] = ".";
            char file[4096];

            if (flag_dir == 1) {

              strcat(direc, "/");
              if (directory[0] != '.')
                strcat(direc, directory);
              else
                strcpy(direc, directory);
            }
            if (mode == 0) {
              disco(direc, dflag, fflag);
              chdir(current);
            } else {
              strcpy(file, direc);
              strcat(file, "/");
              strcat(file, file_n);
              disc(direc, file, file_n);
            }
          } else if ((strcmp(para[0], "jobs") == 0)) {
            my_jobs(para,tot_para-1,bag_proc,bg_inp);
          }else if ((strcmp(para[0], "sig") == 0)) {
            my_sig(para,tot_para-1,bag_proc,bg_inp);
          }else if ((strcmp(para[0], "fg") == 0)) {
            my_foreg(para,tot_para-1,bag_proc,bg_inp);
          }else if ((strcmp(para[0], "bg") == 0)) {
            my_backg(para,tot_para-1,bag_proc,bg_inp);
          }else if ((strcmp(para[0], "exit") == 0)) {
            hfile = fopen(hist_path, "w");
            for (int i = 0; i < count_hist; i++) {
              strcat(array[i], "\n");
              fprintf(hfile, "%s", array[i]);
            }
            fclose(hfile);
            return 0;
          } else if (is_bg == 1) {

            char *arguments[4096];

            for (int i = 0; i < tot_para - 1; i++) {
              arguments[i] = para[i];
            }

            bg(bag_proc, bg_inp, arguments);
          } else {
            char *arguments[4096];

            for (int i = 0; i < tot_para - 1; i++) {
              arguments[i] = para[i];
            }

            tim = fg(arguments);
          }

        if(isredirectedout==1 || isredirectedout==2 ){
          close(file_desc);
          dup2(saved, STDOUT_FILENO);
          close(saved);
        }
        if(isredirectedin==1){
          close(inp_file_desc);
          dup2(saved_inp, STDIN_FILENO);
          close(saved_inp);
        }
        if (pipe_flag == 1) {

        dup2(shell_out, STDOUT_FILENO);
        close(tube[1]);
        dup2(tube[0], STDIN_FILENO);
        close(tube[0]);
        }
      }
      if (pipe_flag == 1) {

      dup2(shell_in, STDIN_FILENO);
    dup2(shell_out, STDOUT_FILENO);}

    }
  }

  }

  return 0;
}
