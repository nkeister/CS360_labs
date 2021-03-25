/************* t.c file ********************/
#include <stdio.h>
#include <stdlib.h>

int *FP;

int main(int argc, char *argv[ ], char *env[ ])
{
  int a,b,c;
    printf("enter main\n");
	    
    printf("&argc=%x argv=%x env=%x\n", &argc, argv, env);
    printf("&a=%8x &b=%8x &c=%8x\n", &a, &b, &c);

//(1). Write C code to print values of argc and argv[] entries-------
int i;
    //printf("counter:  %d\n",argc);
    //printf("argv: ");
    for(i=1;i<argc;i++)
    {
        printf("  %s",argv[i]);	//print argv values stored in outfile
    }
	printf("\n");    
//--------------------------------------------------------------------  
  a=1; b=2; c=3;
   //A(a,b);
  printf("exit main\n");
}

int A(int x, int y)
{
  int d,e,f;
  printf("enter A\n");

// write C code to PRINT ADDRESS OF d, e, f%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
  d=4; e=5; f=6;
  printf("Address of d: %x Address of e: %x Address of f: %x \n", &d, &e, &f);
//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
   
   B(d,e);
  printf("exit A\n");
}

int B(int x, int y)
{
  int g,h,i;
  printf("enter B\n");
  
  // write C code to PRINT ADDRESS OF g,h,i%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
  g=7; h=8; i=9;	
  printf("Address of g: %x Address of h: %x Address of i: %x \n", &g, &h, &i);
  //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

  C(g,h);
  printf("exit B\n");
}

int C(int x, int y)
{
  int u, v, w, i, *p;

  printf("enter C\n");
  
  // write C cdoe to PRINT ADDRESS OF u,v,w,i,p;&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
  u=10; v=11; w=12; i=13;	
  printf("Address of u: %x Address of v: %x Address of w: %x Address of i: %x Address of p: %x  \n", &u, & v, &w, &i, &p);
  //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

  FP = (int *)getebp();  // FP = stack frame pointer of the C() function
	p = (int *)&p;

  printf("exit C\n");
while(p!=0){printf("the stack frame link list:\n", &FP);
	
}

 // for(int i = 0; i < 128; i++)
  {
	  printf("%x  ", &p);
  }
};