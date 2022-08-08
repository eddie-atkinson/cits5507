#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// I know the math stdlib is weird cross platform, this is much easier
int min(int x, int y) {
  if (x > y) {
    return x;
  }
  return y;
}

void printArr(int* arr, int arrSize) {
  for (int i = 0; i < arrSize; ++i) {
    printf("%d ", arr[i]);
  }
  printf("\n");
}

void printArrSlice(int* arr, int startingIdx, int sliceSize) {
  int stopIdx = startingIdx + sliceSize;
  for (int i = startingIdx; i < stopIdx; ++i) {
    printf("%d ", arr[i]);
  }
  printf("\n");
}
int* bubbleSort(int* const inputArr, int arrSize) {
  int* sortedArr = malloc(sizeof(int) * arrSize);
  memcpy(sortedArr, inputArr, arrSize * sizeof(int));
  for (int i = 0; i < arrSize; ++i) {
    for (int j = 0; j < arrSize - i - 1; ++j) {
      int curr = sortedArr[j];
      int next = sortedArr[j + 1];

      if (curr > next) {
        sortedArr[j] = next;
        sortedArr[j + 1] = curr;
      }
    }
  }
  return sortedArr;
}

void copyArr(int* const inputArr, int* const sortedArr, int arrSize) {
  for (int i = 0; i < arrSize; ++i) {
    inputArr[i] = sortedArr[i];
  }
}

void merge(int left, int right, int* const inputArr, int length) {
  int k = left;
  bool leftRemaining = true;
  bool rightRemaining = true;
  int leftIdx = left;
  int rightIdx = right;
  printArrSlice(inputArr, left, length);
  printArrSlice(inputArr, right, length);
  while (leftRemaining && rightRemaining) {
    int leftVal = inputArr[leftIdx];
    int rightVal = inputArr[rightIdx];

    if (leftVal < rightVal && leftRemaining) {
      inputArr[k] = leftVal;
      ++leftIdx;
    } else {
      inputArr[k] = rightVal;
      ++rightIdx;
    }

    leftRemaining = leftIdx < left + length;
    rightRemaining = rightIdx < right + length;
    ++k;
  }
}

void doMergeSort(int leftIdx, int rightIdx, int* const inputArr, int length) {
  // Sub array is trivially sorted
  if (length == 1) {
    return;
  }

  int leftArrLeftIdx = leftIdx;
  int leftArrRightIdx = rightIdx / 2;

  int rightArrLeftIdx = leftArrRightIdx + 1;
  int rightArrRightIdx = rightIdx;

  doMergeSort(leftArrLeftIdx, leftArrRightIdx, inputArr,
              leftArrRightIdx - leftArrLeftIdx);
  doMergeSort(rightArrLeftIdx, rightArrRightIdx, inputArr,
              rightArrRightIdx - rightArrLeftIdx);

  merge(leftIdx, rightIdx, inputArr, length);
}

int* mergeSort(int* const inputArr, int arrSize) {
  int* sortedArr = malloc(sizeof(int) * arrSize);
  copyArr(inputArr, sortedArr, arrSize);
  doMergeSort(0, arrSize - 1, sortedArr, arrSize);
  return sortedArr;
}