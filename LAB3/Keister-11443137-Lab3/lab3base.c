/************** lab3base.c file **************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include "lab3header.h"
#include "lab3func.c"

#define MAX 128

char gdir[MAX]; // gdir[ ] stores dir strings
char gdir2[MAX];
char *dir[64];
int ndir = 0;

char gpath[MAX]; // gpath[ ] stores token strings
char *name[64];
char *name2[64];
char *HOME;
int ntoken;
FILE *fdopen(int fd, const char *mode);
int sytem(const char *cmd);

void commands();
int tokenize1();
int tokenize2();

int main(int argc, char *argv[], char *env[])
{
  int i, r;
  int pid, status;
  char *s, cmd[64], line[MAX];
  char cmdPath[512];
  initialize(env);

  printf("************* Welcome to NickShell **************\n");
  i = 0;
  while (env[i])
  {
    printf("env[%d] = %s\n", i, env[i]);

    // Looking for PATH=
    if (strncmp(env[i], "PATH=", 5) == 0)
    {
      printf("show PATH: %s\n", env[i]);

      printf("decompose PATH into dir strings in gdir[ ]\n");
      //strcpy(gdir, &env[i][5]);

      /*************** 1 ******************************
      Write YOUR code here to decompose PATH into dir strings in gdir[ ]
      pointed by dir[0], dir[1],..., dir[ndir-1]
      ndir = number of dir strings
      print dir strings
      ************************************************/

      printf("\n-------------Printing env[46] strings:--------------\n");
      for (int i = 0; i < 9; i++)
      {
        tokenize2(env[46]); //token env[46]
        dir[i] = env[46];   //save token of env[46] in dir[i++]
        printf("dir[%d]: %s\n", i, name2[i]);
        ndir++; //counter for size of dir strings
      }
      printf("\nNumber of string ndir's = %d\n", ndir); //return ndir

      break;
    }
    i++;
  }

  printf("*********** kcsh processing loop **********\n");

  while (1)
  {
    printf("kcsh % : ");

    fgets(line, 128, stdin);
    line[strlen(line) - 1] = 0; // fgets() has \n at end

    if (line[0] == 0)
      continue;
    printf("line = %s\n", line); // print line to see what you got

    //***************** 2 **********************
    //  Write YOUR code here to decompose line into token strings in gpath[ ]
    //  pointed by name[0], name[1],..., name[ntoken-1]
    //  ntoken = number of token strings
    //  print the token strings
    //  ************************************************/
    tokenize1(line);
    printf("%s\n", name[0]);
    // 3. Handle name[0] == "cd" or "exit" case

    if (strcmp(name[0], "cd") == 0)
    {
      chdir(HOME);
      printf("PROC %d cd to HOME\n\n", getpid());
      printf("%s", name[0]);
    }
    else if (strcmp(name[0], "exit") == 0)
    {
      exit(1);
    }
    else
    {
      pid = fork(); // fork a child sh
      if (pid < 0)
      {
        fprintf(stderr, "Fork failed!");
        exit(-1);
      }
      if (pid)
      {
        fprintf(stderr, "parent PROC %d forks a child process %d\n", getpid(), pid);
        fprintf(stderr, "parent sh PROC %d waits\n", getpid());
        pid = wait(&status);
        fprintf(stderr, "\nchild sh PROC %d died : exit status = %04x\n", pid, status);
      }
      else
      {
        fprintf(stderr, "PROC %d: line=%s\n", getpid(), line);
        strcpy(head, line); //str copy line to head
        strcpy(tail, "");
        exe(env); //execute command

        exit(0);
      }
    }

    if (pid)
    {
      printf("parent PROC %d forks child %d ", getpid(), pid);
      printf("parent sh %d waits \n", getpid());
      pid = wait(&status); //waits for ZOMBIE child process
      printf("child sh %d died : exit status = %04x\n", pid, status);
      continue;
    }
    else
    {
      strcpy(head, line); //execute second command following | -- global
      strcpy(tail, "");
      exe(env); //enter exe function to execute command

      exit(0);
      printf("child sh %d begins\n", getpid());
      for (i = 0; i < ndir; i++)
      {

        strcpy(cmd, dir[i]);
        strcat(cmd, "/");
        strcat(cmd, name[0]);
        printf("i=%d cmd=%s\n", i, cmd);

        r = execve(cmd, name, env);
      }
      printf("cmd %s not found, child sh exit\n", name[0]);
      exit(123); // die with value 123
    }
  }
}

/*********************** 5 *********************
Write your code to do I/O redirection:
Example: check any (name[i] == ">"). 
         If so, set name[i] = 0; 
                redirecct stdout to name[i+1] 
********************************************/

/******************* 6 ***********************
 Handle pipe: check pipe symbol | in input line;
 if so, divide line into head, tail

 create PIPE, fork a child to share the pipe
 parent write to  pipe and exec head;
 child  read from pipe and exec tail
********************************************/
int tokenize1(char str[])
{
  int token = 0;
  char *n;
  n = strtok(str, ""); // first call to strtok()

  if (!n)
  {
    return 0;
  }
  while (n)
  {
    name[token] = n;
    token++;           //increment tok
    n = strtok(0, ""); // call strtok() until NULL
  }
  return token;
}

int tokenize2(char str[])
{
  int token = 0;
  char *n;

  n = strtok(str, ":"); // first call to strtok()
  if (!n)
  {
    return 0;
  }
  while (n)
  {
    name2[token] = n;
    token++;            //increment tok
    n = strtok(0, ":"); // call strtok() until NULL
  }
  return token;
}