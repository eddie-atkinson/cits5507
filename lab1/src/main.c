#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "sort.h"

// You may need to tweak this number depending on how much RAM your OS allocates
// to processes
#define N_LOOPS 20

// Use a consistent random seed so that results are comparable
#define RANDOM_SEED 42
/*
Q1.
Your computer is so fast that you need to do a lot of computations for the time
to be measurable. First try to write simple loops that execute many times,e.g.,
start with several hundred thousand iterations and increase the number of
iterations gradually. You need not have any statements inside the loop.

Now plot the running times against the number of iterations in a graph. You can
use Microsoft Excel. Does the time increase linearly? In other words, is the
increase in time proportional to the number of iterations?
*/

typedef struct {
  int nIter;
  double runTime;
} RunTime;

// Write an array of run time results to a CSV at a given path
void writeRunTimesCsv(RunTime** const runTimes, int runTimesLength,
                      char* filePath) {
  RunTime** runTimeWalker = runTimes;

  FILE* fp = fopen(filePath, "w");
  if (fp == NULL) {
    printf("Cannot open %s, does that path exist?\n", filePath);
    exit(EXIT_FAILURE);
  }

  fprintf(fp, "iteration,execution_time_s\n");

  for (int i = 0; i < runTimesLength; ++i) {
    RunTime* runTime = *runTimeWalker;
    fprintf(fp, "%d,%f\n", runTime->nIter, runTime->runTime);
    ++runTimeWalker;
  }
  fclose(fp);
}

void runTrivialLoop(int nLoops) {
  for (int i = 0; i < nLoops; ++i) {
    // Do nothing
  }
}

// Run trivial loops of varying lengths, record their speed and write it a CSV
// file
void checkTrivialLoops() {
  clock_t begin;
  clock_t end;
  double time_spent;

  // Allocate memory for array of run time structs
  RunTime** runTimes = malloc(sizeof(RunTime*) * N_LOOPS);

  for (int i = 0; i < N_LOOPS; ++i) {
    // Take 2^i as the loop length
    int loopLength = 2 << i;

    // Run loop and time execution
    begin = clock();
    runTrivialLoop(loopLength);
    end = clock();
    time_spent = (double)(end - begin) / CLOCKS_PER_SEC;

    runTimes[i] = malloc(sizeof(RunTime));
    runTimes[i]->nIter = loopLength;
    runTimes[i]->runTime = time_spent;

    printf("CPU Execution time for loop of length %d was: %fs\n", loopLength,
           time_spent);
  }

  // Write results to CSV for analysis
  writeRunTimesCsv(runTimes, N_LOOPS, "./results/trivial_loop.csv");

  // Free the memory we allocated along the way
  for (int i = 0; i < N_LOOPS; ++i) {
    free(runTimes[i]);
  }
  free(runTimes);
}

// Now write a program that does something non-trivial inside a loop. For
// example, declare a large enough array and store random floating point numbers
// between 0 and 1 in the array. Now add these floating point numbers in the
// loop.
// NOTE: I just did large numbers, much more useful for later exercises

int* generateRandomNumberArray(int arrSize, int initialSeed) {
  srand(initialSeed);
  int* arr = malloc(sizeof(int) * arrSize);
  for (int i = 0; i < arrSize; ++i) {
    arr[i] = rand();
  }
  return arr;
}

void runNonTrivialLoop(int nLoops) {
  int sum = 0;
  int* randomNums = generateRandomNumberArray(nLoops, RANDOM_SEED);
  for (int i = 0; i < nLoops; ++i) {
    sum += randomNums[i];
  }
  free(randomNums);
}

void checkNonTrivialLoops() {
  clock_t begin;
  clock_t end;
  double time_spent;

  // Allocate memory for array of run time structs
  RunTime** runTimes = malloc(sizeof(RunTime*) * N_LOOPS);

  for (int i = 0; i < N_LOOPS; ++i) {
    // Take 2^i as the loop length
    int loopLength = 2 << i;

    // Run loop and time execution
    begin = clock();
    runNonTrivialLoop(loopLength);
    end = clock();
    time_spent = (double)(end - begin) / CLOCKS_PER_SEC;

    runTimes[i] = malloc(sizeof(RunTime));
    runTimes[i]->nIter = loopLength;
    runTimes[i]->runTime = time_spent;

    printf("CPU Execution time for loop of length %d was: %fs\n", loopLength,
           time_spent);
  }

  // Write results to CSV for analysis
  writeRunTimesCsv(runTimes, N_LOOPS, "./results/non_trivial_loop.csv");

  // Free the memory we allocated along the way
  for (int i = 0; i < N_LOOPS; ++i) {
    free(runTimes[i]);
  }
  free(runTimes);
}

// Q2. Write the bubble sort algorithm in C and check timing with larger and
// larger input sizes. There are many sources that you can look up for the
// bubble sort algorithm.

void checkSortingAlg(int* (*sortingFunc)(int*, int), char* algorithmName) {
  clock_t begin;
  clock_t end;
  double time_spent;

  // Allocate memory for array of run time structs
  RunTime** runTimes = malloc(sizeof(RunTime*) * N_LOOPS);

  for (int i = 0; i < N_LOOPS; ++i) {
    // Take 2^i as the loop length
    int arrSize = 2 << i;
    int* arr = generateRandomNumberArray(arrSize, RANDOM_SEED);

    // Run sorting alg and time execution
    begin = clock();
    int* sortedArr = sortingFunc(arr, arrSize);
    end = clock();
    free(arr);
    free(sortedArr);
    time_spent = (double)(end - begin) / CLOCKS_PER_SEC;

    runTimes[i] = malloc(sizeof(RunTime));
    runTimes[i]->nIter = arrSize;
    runTimes[i]->runTime = time_spent;

    printf(
        "CPU Execution time for array of length %d with algorithm %s was: "
        "%fs\n",
        arrSize, algorithmName, time_spent);
  }

  // Isn't C an absolute peach?
  char* filePathPrefix = "./results/";
  int suffixLength = strlen(".csv") + strlen(algorithmName);
  char* fullFilePath =
      malloc((strlen(filePathPrefix) + suffixLength) * sizeof(char));
  sprintf(fullFilePath, "%s%s%s", filePathPrefix, algorithmName, ".csv");

  // Write results to CSV for analysis
  writeRunTimesCsv(runTimes, N_LOOPS, fullFilePath);
  // Free the memory we allocated along the way
  for (int i = 0; i < N_LOOPS; ++i) {
    free(runTimes[i]);
  }
  free(runTimes);
}
void checkSortingAlgs() {
  checkSortingAlg(bubbleSort, "bubble_sort");
  printf("\n");
  checkSortingAlg(mergeSort, "merge_sort");
  printf("\n");
  checkSortingAlg(quickSort, "quick_sort");
  printf("\n");
}

int main(int argc, char** argv) {
  checkTrivialLoops();
  printf("\n");
  checkNonTrivialLoops();
  printf("\n");
  checkSortingAlgs();

  return EXIT_SUCCESS;
}