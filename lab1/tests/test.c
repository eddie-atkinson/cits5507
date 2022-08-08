
#include "test.h"

int runTestCase(TestCase* testCase, int* (*f)(int*, int)) {
  int failed = 0;
  int* sorted = f(testCase->input, testCase->length);
  for (int i = 0; i < testCase->length; ++i) {
    int sortedVal = sorted[i];
    if (testCase->sorted[i] != sortedVal) {
      failed = 1;
      printf(C_RED("%d "), sortedVal);
    } else {
      printf(C_GREEN("%d "), sortedVal);
    }
  }
  printf("\n");
  free(sorted);
  return failed;
}

void testSort(char* algorithmName, int* (*f)(int*, int)) {
  int nFailed = 0;
  printf(C_MAGENTA("Testing algorithm ") " %s\n", algorithmName);
  int nTestCases = (sizeof(testCases) / sizeof(TestCase));

  for (int i = 0; i < nTestCases; ++i) {
    TestCase testCase = testCases[i];
    int failed = runTestCase(&testCase, f);
    if (failed) {
      printf(C_RED("Failed test case: ") " %s\n", testCase.name);
      printf("Input arr: ");
      printArr(testCase.input, testCase.length);
      printf("\n");
    } else {
      printf(C_GREEN("Passed test case: ") " %s\n\n", testCase.name);
    }
    nFailed += failed;
  }
  if (nFailed > 0) {
    printf(C_YELLOW("Failed %d test cases\n\n"), nFailed);
  } else {
    printf(C_GREEN("Passed all test cases!\n\n"));
  }
}

int main() {
  testSort("Bubble Sort", bubbleSort);
  testSort("Merge Sort", mergeSort);
  return EXIT_SUCCESS;
}