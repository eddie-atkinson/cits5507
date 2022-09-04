#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include "omp.h"

#define MAX_TEST_THREADS 128
#define LARGE_ARR_SIZE (2 ^ 18)
#define LARGE_ITER_COUNT (2 ^ 25)

// Use the #pragma omp parallel and omp_get_thread_num() in this exercise. Write
// a multi-threaded program in which each thread will write its thread number
// and some meaningful message. Run the program several time. Do you see the
// same ordering of the print statements? The scheduling of the CPUs are
// actually non-deterministic if there are other processes in the system. Hence
// the ordering of the print statements should be random (although there are
// other factors too).

void checkThreadPrinting() {
  clock_t begin;
  clock_t end;
  double time_spent;

  begin = clock();
#pragma omp parallel for
  for (int i = 0; i < 10; ++i) {
    printf("Checking in from thread %d for iteration: %d\n",
           omp_get_thread_num(), i);
  }
  end = clock();
  time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
  printf("time spent = %10.6f\n", time_spent);
}

// Now modify the program so that one or more threads sleep for some time. Use
// the sleep(1) call. This is a system call that makes a thread to sleep. How do
// you choose a particular thread to sleep? You can do this in an if statement
// using the omp_get_thread_num() directive. What is the ordering of the print
// statements now?
void sleepyThreads() {
#pragma omp parallel for
  for (int i = 0; i < 10; ++i) {
    int threadNum = omp_get_thread_num();
    // Sleep for odd numbered threads
    if (threadNum % 2) {
      sleep(1);
    }
    printf("Checking in from thread %d for iteration: %d\n", threadNum, i);
  }
}

void orderedThreadSleep() {
#pragma omp parallel for
  for (int i = 0; i < 10; ++i) {
    int threadNum = omp_get_thread_num();
    sleep(threadNum);
    printf("Checking in from thread %d for iteration: %d\n", threadNum, i);
  }
}

// Now write a C program where you will change the number of threads used by
// omp_set_num_threads(n) directive. This directive should appear before a
// #pragma omp parallel directive and then the parallel region will be executed
// by n threads, where n is an integer. There is usually a limit how many
// threads you can launch from a process in a linux system. I have tested up to
// 512 threads in my machine, but we will work with smaller number of threads.
void loopWithNThreads(int nThreads) {
  omp_set_num_threads(nThreads);
  int nIter = 2 * nThreads;
#pragma omp parallel for
  for (int i = 0; i < nIter; ++i) {
    printf("Running in thread %d, iteration: %d\n", omp_get_thread_num(), i);
  }
}

// Now write a program that adds floating point numbers stored in a large array.
// This is our first non-trivial OpenMP program. Each thread will use a local
// variable called localSum to store the sum that it computes in its part of the
// for loop. Hence, we have to make the variable localSum a private variable for
// each thread. This is done by changing the #pragma omp parallel to #pragma omp
// parallel private(localSum). The rest of the code will be similar to what you
// would otherwise write for summing numbers in a loop. Use the localSum
// variable to store the sum of each thread. Remember, each thread has its own
// private copy of localSum variable now.

int* generateRandomNumberArray(int arrSize, int initialSeed) {
  srand(initialSeed);
  int* arr = (int*)malloc(sizeof(int) * arrSize);
  for (int i = 0; i < arrSize; ++i) {
    arr[i] = rand();
  }
  return arr;
}

int sumLargeRandomArray(int* arr, int arrSize, int nThreads) {
  omp_set_num_threads(nThreads);
  int localSums[nThreads];
  memset(&localSums, 0, sizeof(int) * nThreads);
#pragma omp parallel for
  for (int i = 0; i < arrSize; ++i) {
    int threadNum = omp_get_thread_num();
    localSums[threadNum] += arr[i];
  }
  int totalSum = 0;
  for (int i = 0; i < nThreads; ++i) {
    totalSum += localSums[i];
  }
  return totalSum;
}

int sumArrFast(int* arr, int arrSize) {
  int sum = 0;
#pragma omp parallel for reduction(+ : sum)
  for (int i = 0; i < arrSize; ++i) {
    sum += arr[i];
  }
  return sum;
}

void testSumLargeRandomArray(void) {
  int* randomArr = generateRandomNumberArray(LARGE_ARR_SIZE, 42);
  clock_t begin;
  clock_t end;
  double time_spent;
  int totalSum = sumArrFast(randomArr, LARGE_ARR_SIZE);

  for (int i = 2; i < MAX_TEST_THREADS; i *= 2) {
    begin = clock();
    int randomSum = sumLargeRandomArray(randomArr, LARGE_ARR_SIZE, i);
    end = clock();

    time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("time spent for %d threads = %10.6f\n", i, time_spent);
    if (totalSum == randomSum) {
      printf("Total sum is correct!\n");
    } else {
      printf("Total sum is incorrect!\n");
    }
  }
  free(randomArr);
}

// Quick sort stuff

void swapIndices(int* const inputArr, int indexOne, int indexTwo) {
  int swap = inputArr[indexOne];
  inputArr[indexOne] = inputArr[indexTwo];
  inputArr[indexTwo] = swap;
}

int partition(int* const inputArr, int low, int high) {
  int x = inputArr[high];
  int i = low - 1;
  for (int j = low; j < high; ++j) {
    if (inputArr[j] <= x) {
      ++i;
      swapIndices(inputArr, i, j);
    }
  }
  ++i;
  swapIndices(inputArr, i, high);
  return i;
}

// Implement tail recursive quick sort function from CLRS
void doQuickSort(int* const inputArr, int low, int high) {
  while (low < high) {
    int pivot = partition(inputArr, low, high);
    doQuickSort(inputArr, low, pivot - 1);
    low = pivot + 1;
  }
}

int* quickSort(int* const inputArr, int arrSize) {
  int* sortedArr = (int*)malloc(sizeof(int) * arrSize);
  memcpy(sortedArr, inputArr, arrSize * sizeof(int));
  doQuickSort(sortedArr, 0, arrSize - 1);
  return sortedArr;
}

// Write a separate program where you call a sorting function from inside a for
// loop with a large number of iterations. You can make the sorting program that
// you wrote in the last lab as a function and call that function from inside
// the for loop. This is to introduce more work for the threads. Each iteration
// of the for loop will sort the same array, as we are just interested in making
// the threads work harder. Time this program and do a similar analysis for
// sorting larger and larger arrays many many times (increasing number of
// iterations of the for loop), and also with diferent number of threads.

void sortArrWithNThreads(int* arr, int arrSize, int nThreads) {
  omp_set_num_threads(nThreads);
#pragma omp parallel
  for (int i = 0; i < LARGE_ITER_COUNT; ++i) {
    quickSort(arr, arrSize);
  }
}

void testSortLargeArrayVaryingThreads() {
  int* randomArrs[20];
  for (int i = 0; i < 20; ++i) {
    randomArrs[i] = generateRandomNumberArray(2 << i, 42);
  }

  clock_t begin;
  clock_t end;
  double time_spent;
  for (int i = 2; i < MAX_TEST_THREADS; i *= 2) {
    printf("Run time with %d threads:\n", i);

    for (int j = 0; j < 20; ++j) {
      int arrSize = 2 << j;
      begin = clock();
      sortArrWithNThreads(randomArrs[j], arrSize, i);
      end = clock();
      time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
      printf("\ttime spent for array size %d = %10.6f\n", arrSize, time_spent);
    }
  }
  for (int i = 0; i < 20; ++i) {
    free(randomArrs[i]);
  }
}

int main(int argc, char* argv[]) {
  //   checkThreadPrinting();
  //   sleepyThreads();
  //   orderedThreadSleep();
  //   loopWithNThreads(2);
  //   printf("\n");
  //   loopWithNThreads(8);
  //   printf("\n");
  //   loopWithNThreads(64);
  //   printf("\n");
  //   testSumLargeRandomArray();
  testSortLargeArrayVaryingThreads();
  //   Seems like the overhead of managing more threads increases runtime in a
  //   non-linear fashion
  return 0;
}