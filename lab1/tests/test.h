#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../src/sort.h"

#if _WIN32
#define C_RED(s) s
#define C_GREEN(s) s
#define C_YELLOW(s) s
#define C_MAGENTA(s) s
#else
#define C_RED(s) "\033[31;1m" s "\033[0m"
#define C_GREEN(s) "\033[32;1m" s "\033[0m"
#define C_YELLOW(s) "\033[33;1m" s "\033[0m"
#define C_MAGENTA(s) "\033[35;1m" s "\033[0m"
#endif

typedef struct {
  int length;
  int* input;
  int* sorted;
  char* name;
} TestCase;

int trivial[] = {0};
int reverse[] = {9, 8, 7, 6, 5, 4, 3, 2, 1, 0};
int reverseSorted[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
int oddNumberOfElements[] = {9, 1, 7, 3, 0};
int oddNumberOfElementsSorted[] = {0, 1, 3, 7, 9};
int dupes[] = {2, 2, 3, 5, 5, 1};
int dupesSorted[] = {1, 2, 2, 3, 5, 5};

TestCase testCases[] = {
    {1, trivial, trivial, "trivial"},
    {10, reverse, reverseSorted, "reverse"},
    {10, reverseSorted, reverseSorted, "already sorted"},
    {5, oddNumberOfElements, oddNumberOfElementsSorted,
     "odd number of elements"},
    {6, dupes, dupesSorted, "duplicate entries"},

};
