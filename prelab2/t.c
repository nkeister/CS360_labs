/*********** A Multitasking System ************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "type.h"
#include "queue.c"
#include "wait.c"

struct node
{
  int key;
  struct node *left, *right;
};

PROC proc[NPROC], *running, *freeList, *readyQueue;
PROC *sleepList;  // for sleep()/wakeup() 

int body()
{
  char command[128];
  printf("proc %d resume to body()\n", running->pid);
  while(1){
    printf("***************************************\n");
    printf("proc %d running: Parent=%d\n", running->pid, running->ppid);

    printList("freeList ", freeList);
    printList("readQueue", readyQueue);
    
    printf("input a command: [ps|fork|switch|exit|sleep] : ");
    fgets(command, 64, stdin);
    command[strlen(command)-1] = 0;
     
    if (!strcmp(command, "ps"))
      do_ps();
    else if (!strcmp(command, "sleep"))
      do_sleep();
    else if (!strcmp(command, "fork"))
      do_fork();
    else if (!strcmp(command, "switch"))
      do_switch();
    else if (!strcmp(command, "exit"))
      do_exit();
    else
      printf("invalid command\n");
  }
}
//%%%%%%%%%%%%%%%%%%%%%%%%% do_sleep %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
int do_sleep()
{
  PROC *p;

  ksleep(p);
}//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

int do_switch()
{
   printf("proc %d switch task\n", running->pid);
   tswitch();
   printf("proc %d resume\n", running->pid);
}

int do_fork()
{
   int child = kfork(body);
   if (child < 0)
      printf("kfork failed\n");
   else{
      printf("proc %d kforked a child = %d\n", running->pid, child);
      printList("readyQueue", readyQueue);
   }
   return child;
}

char *pstatus[]={"FREE   ","READY  ","SLEEP  ","BLOCK  ","ZOMBIE", "RUNNING"};
int do_ps()
{
  int i; PROC *p;
  printf("pid   ppid    status\n");
  printf("--------------------\n");
  for (i=0; i<NPROC; i++){
     p = &proc[i];
     printf(" %d      %d     ", p->pid, p->ppid);
     if (p==running)
       printf("%s\n", pstatus[5]);
     else
       printf("%s\n", pstatus[p->status]);
  }
}

// int kexit(int exitValue)
// {
//   running->exitCode = value;
//   running->status = ZOMBIE;
//   tswitch(); 
// } 

int do_exit()
{
  int value;
  PROC *p;
  if (running->pid==1){
      printf("P1 never dies\n");
      return 0;
  }
  printf("proc %d in do_exit(), enter an exit value : ", running->pid);
  scanf("%d", &value);
  kexit(value);
} 
//%%%%%%%%%%%%%%%%%%%%%%% Create new node in BST %%%%%%%%%%%%%%%%%%%%
struct node *newNode(int item)//Creates new BST node
{
  struct node *temp = (struct node *)malloc(sizeof(struct node));
  temp->key = item;
  temp->left = temp->right = NULL;
  return temp;
}
struct node* insert(struct node* node, int key)//insert node in BST
{
  if(node == NULL)//check if BST is empty
    return newNode(key);//create a new node if NULL and return

  if(key < node->key)//check if key is less than node
    node->left = insert(node->left, key);//insert key left of node

  else if(key > node->key)//check if key is greater than node
    node->right = insert(node->right, key);//insert key right of node
  return node;
}
struct node* inorder(struct node *root)
{
  if(root != NULL)
  {
    inorder(root->left);
    printf("test: ");
    printf("%d \n", root->key);
    inorder(root->right);
  }
}
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
int kfork(int (*func))
{
  struct node *root = NULL;
  PROC *p;
  int  i;
  // get a proc from freeList for child proc:
  p = dequeue(&freeList);
  if (!p){
     printf("no more proc\n");
     return(-1);
  }

  // // initialize the new proc and its stack
  p->status = READY;
  p->priority = 1;         // for ALL PROCs except P0
  p->ppid = running->pid;
  p->parent = running;

  //%%%%%%%%%%%%%%%%%%%% Modifty BST here %%%%%%%%%%%%%%%%%%%%%%%%%%%
  //                    -1   -2  -3  -4  -5  -6  -7  -8   -9
  // kstack contains: |retPC|eax|ebx|ecx|edx|ebp|esi|edi|eflag|
   for (i=1; i<10; i++)
    p->kstack[SSIZE - i] = 0;
    p->kstack[SSIZE-1] = (int)func;
    p->saved_sp = &(p->kstack[SSIZE - 9]);
  //insert(root, p);
  //inorder(root);

  enqueue(&readyQueue, p);
  //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
  return p->pid;
}
int init()
{
  int i;
  for (i = 0; i < NPROC; i++){
    proc[i].pid = i; 
    proc[i].status = FREE;
    proc[i].priority = 0;
    proc[i].next = (PROC *)&proc[(i+1)];
  }
  proc[NPROC-1].next = 0;
 
  freeList = &proc[0];        
  readyQueue = 0;

  // create P0 as the initial running process
  running = dequeue(&freeList);
  running->status = READY;
  running->priority = 0;
  running->parent = running;
  printList("freeList", freeList);
  printf("init complete: P0 running\n"); 
}

/*************** main() ***************/
int main()
{
   printf("\nWelcome to Nicks 360 Multitasking System\n");
   init();
   printf("P0 fork P1\n");
   kfork(body);  

   while(1){
     if (readyQueue){
        printf("P0: switch task\n");
        tswitch();
     }
   }
}

/*********** scheduler *************/
int scheduler()
{ 
  printf("proc %d in scheduler()\n", running->pid);
  if (running->status == READY)
      enqueue(&readyQueue, running);
  printList("readyQueue", readyQueue);
  running = dequeue(&readyQueue);
  printf("next running = %d\n", running->pid);  
}


