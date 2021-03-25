/** C4.5.c: Gauss Elimination with Partial Pivoting **/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <pthread.h>
#define _XOPEN_SOURCE 600
#define N 8


double A[N][N + 1];
int counter, h;

pthread_barrier_t barrier;
pthread_cond_t cond1 = PTHREAD_COND_INITIALIZER;

void threadfunc();   //function prototype for initializing # of threads
int print_matrix();  //printf matrix function
void *ge(void *arg); // threads function: Gauss elimination

int main(int argc, char *argv[])
{
   int i, j;
   int g, f;
   double sum;

   //%%%%%%%%%%%%%%%%%% Enter N threads %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   printf("Enter N number of threads: ");
   scanf("%d", &h); //get user input of threads
    threadfunc();


   printf("\n");
   return 0;
}

void threadfunc()
{
   int i, j;
   int g, f;
   double sum;

   //%%%%%%%%%%%%%%%%%% Enter N threads %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   //printf("Enter N number of threads: ");
   //scanf("%d", &h); //get user input of threads

   pthread_t threads2[N]; //number of threads to create
   //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

   printf("main: initialize matrix A[N][N+1] as [A|B]\n");
   for (i = 0; i < N; i++)
      for (j = 0; j < N; j++)
         A[i][j] = 1.0;
   for (i = 0; i < N; i++)
      A[i][N - i - 1] = 1.0 * N;
   for (i = 0; i < N; i++)
   {
      A[i][N] = (N * (N + 1)) / 2 + (N - i) * (N - 1);
   }
   print_matrix(); // show initial matrix [A|B]

   pthread_barrier_init(&barrier, NULL, N); // set up barrier with 'h' number of threads

   printf("main: create N=%d working threads\n", h); //change to 'h' # threads
   for (i = 0; i < N; i++)                           //changed loop from 0 to 'h' number of threads
   {
      pthread_create(&threads2[i % h], NULL, ge, (void *)i); //create thread with stack size
   }

   printf("main: wait for all %d working threads to join\n", h);
   for (i = 0; i < N; i++) //change loop from N threads to 'h' # of threads
   {
      pthread_join(threads2[i % h], NULL);//change thread to stack size
   }
   printf("main: back substitution : ");
   for (i = N - 1; i >= 0; i--)
   {
      sum = 0.0;
      for (j = i + 1; j < N; j++)
         sum += A[i][j] * A[j][N];
      A[i][N] = (A[i][N] - sum) / A[i][i];
   }
   // print solution
   printf("The solution if only using %d number of threads: \n", h);
   for (i = 0; i < N; i++)
   {
      printf("%6.2f  ", A[i][N]);
   }
   printf("\n\n\n\n");
   //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
}

void *ge(void *arg) // threads function: Gauss elimination
{
   int i, j, k, prow;
   int myid = (int)arg;
   double temp, factor;

   for (i = 0; i < N - 1; i++)
   {
      if (i == myid)
      {
         printf("partial pivoting by thread %d on row %d: ", (myid%h), i); 
         temp = 0.0;
         prow = i;
         for (j = i; j <= N; j++)
         {
            if (fabs(A[j][i]) > temp)
            {
               temp = fabs(A[j][i]);
               prow = j;
            }
         }
         printf("pivot_row=%d  pivot=%6.2f\n", prow, A[prow][i]);
         if (prow != i)
         { // swap rows
            for (j = i; j < N + 1; j++)
            {
               temp = A[i][j];
               A[i][j] = A[prow][j];
               A[prow][j] = temp;
            }
         }
      }
      // wait for partial pivoting done
      pthread_barrier_wait(&barrier);
      for (j = i + 1; j < N; j++)
      {
         if (j == myid)
         {
            printf("thread %d do row %d\n", (myid%h), j);
            factor = A[j][i] / A[i][i];
            for (k = i + 1; k <= N; k++)
               A[j][k] -= A[i][k] * factor;
            A[j][i] = 0.0;
         }
      }
      // wait for current row reductions to finish
      pthread_barrier_wait(&barrier);
      if (i == myid)
         print_matrix();
   }
}

int print_matrix()
{
   int i, j;
   printf("------------------------------------\n");
   for (i = 0; i < N; i++)
   {
      for (j = 0; j < N + 1; j++)
         printf("%6.2f  ", A[i][j]);
      printf("\n");
   }
}