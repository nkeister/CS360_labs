
/****** C4.2.c: quicksort by threads *****/
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>

typedef struct{
  int upperbound;
  int lowerbound;
}PARM;  

#define N 10        

int a[N] = {5,1,6,4,7,2,9,8,0,3};  // unsorted data
int arr[] = {5,1,6,4,7,2,9,8,0,3};

int print()   // print current a[] contents
{
  int i;
  printf("[ ");
  for (i=0; i<N; i++)
    printf("%d ", a[i]);
  printf("]\n");
}

void swap(int *a, int *b)
{
  int t = *a;
  *a = *b;
  *b = t;
}

int partition(int array[], int low, int high)
{
  int pivot = array[high];
  int i = (low - 1);
  for (int j = low; j < high; j++)
  {
    if (array[j] <= pivot)
    {
      i++;
      swap(&array[i], &array[j]);
    }
  }
  swap(&array[i + 1], &array[high]);
  return (i + 1);
}

void quickSort(int array[], int low, int high)
{
  if (low < high)
  {
    int pi = partition(array, low, high);
    quickSort(array, low, pi - 1);
    quickSort(array, pi + 1, high);
  }
}

void *Qsort(void *aptr)
{
    PARM *ap, aleft, aright;
    int pivot, pivotIndex, left, right, temp;
    int upperbound, lowerbound;

    pthread_t me, leftThread, rightThread;
    me = pthread_self();
    ap = (PARM *)aptr;

    upperbound = ap->upperbound;
    lowerbound = ap->lowerbound;

    pivot = a[upperbound];         // pick low pivot value

    left = lowerbound - 1;         // scan index from left side
    right = upperbound;            // scan index from right side

    if (lowerbound >= upperbound)
       pthread_exit(NULL);


    while (left < right){         // partition loop
       do { left++;} while (a[left] < pivot);

       do { right--;} while (a[right] > pivot);

       if (left < right ){          
          temp = a[left];
          a[left] = a[right];
          a[right] = temp;
       }
    }
    print();

    pivotIndex = left;           // put pivot back
    temp = a[pivotIndex];
    a[pivotIndex] = pivot;
    a[upperbound] = temp;

    // start the "recursive threads"
    aleft.upperbound = pivotIndex - 1;
    aleft.lowerbound = lowerbound;
    aright.upperbound = upperbound;
    aright.lowerbound = pivotIndex + 1;

    printf("%lu: create left and right threads\n", me);
    pthread_create(&leftThread,  NULL, Qsort, (void *)&aleft);
    pthread_create(&rightThread, NULL, Qsort, (void *)&aright); 

    // wait for left and right threads to finish
    pthread_join(leftThread, NULL);
    pthread_join(rightThread, NULL);
    printf("%lu: joined with left & right threads\n", me);
}

void printArray2(int array[], int size)
{
  for (int i = 0; i < size; ++i)
  {
    printf("%d  ", array[i]);
  }
  printf("\n");
}

int main(int argc, char *argv[])
{
    PARM arg;
    int i, *array;
    pthread_t me, thread;

    struct timeval t1, t2, t3, t4;
    gettimeofday(&t1, NULL);

    me = pthread_self();
    printf("main %lu: unsorted array = ", me);
    print();

    arg.upperbound = N-1;
    arg.lowerbound = 0;

    printf("main %lu create a thread to do QS\n", me);
    pthread_create(&thread, NULL, Qsort, (void *)&arg);

    // wait for QS thread to finish
    pthread_join(thread, NULL);
    printf("main %lu sorted array = ", me);
    print();
    
    gettimeofday(&t2, NULL);

    printf("t2: sec=%ld usec=%ld\n", t2.tv_sec, t2.tv_usec);
    printf("t1: sec=%ld usec=%ld\n", t1.tv_sec, t1.tv_usec);
    printf("usec used = %ld\n", t2.tv_usec - t1.tv_usec);

  gettimeofday(&t3, NULL);
  int n = sizeof(arr) / sizeof(arr[0]);
    
    printf("quicksort:");
    quickSort(arr, 0, n -1);
    printArray2(arr, n);
    gettimeofday(&t4, NULL);
   
    //printf("t3: sec=%ld usec=%ld\n", t3.tv_sec, t3.tv_usec);
    printf("usec used = %ld\n", t4.tv_usec - t3.tv_usec);
    return 0;
}

