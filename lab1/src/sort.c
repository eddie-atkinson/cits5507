#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

void merge(int* const inputArr, int left, int middle, int right) {
  int leftLength = middle - left + 1;
  int rightLength = right - middle;

  int leftArr[leftLength];
  int rightArr[rightLength];

  for (int i = 0; i < leftLength; ++i) {
    leftArr[i] = inputArr[i + left];
  }
  for (int i = 0; i < rightLength; ++i) {
    rightArr[i] = inputArr[i + middle + 1];
  }

  int leftIdx = 0;
  int rightIdx = 0;
  int k = left;

  while (leftIdx < leftLength && rightIdx < rightLength) {
    int leftVal = leftArr[leftIdx];
    int rightVal = rightArr[rightIdx];
    if (leftVal < rightVal) {
      inputArr[k] = leftVal;
      ++leftIdx;
    } else {
      inputArr[k] = rightVal;
      ++rightIdx;
    }
    ++k;
  }
  while (leftIdx < leftLength) {
    inputArr[k] = leftArr[leftIdx];
    ++k;
    ++leftIdx;
  }
  while (rightIdx < rightLength) {
    inputArr[k] = rightArr[rightIdx];
    ++k;
    ++rightIdx;
  }
}

void doMergeSort(int* const inputArr, int left, int right) {
  if (left < right) {
    int middle = left + (right - left) / 2;
    doMergeSort(inputArr, left, middle);
    doMergeSort(inputArr, middle + 1, right);
    merge(inputArr, left, middle, right);
  }
}

int* mergeSort(int* const inputArr, int arrSize) {
  int* sortedArr = malloc(sizeof(int) * arrSize);
  memcpy(sortedArr, inputArr, arrSize * sizeof(int));
  doMergeSort(sortedArr, 0, arrSize - 1);
  return sortedArr;
}

void swapIndices(int* const inputArr, int indexOne, int indexTwo) {
  int swap = inputArr[indexOne];
  inputArr[indexOne] = inputArr[indexTwo];
  inputArr[indexTwo] = swap;
}

// Basic divide and conquer partition alg
// TODO: Implement median of three or similar approach to improve performance
// here
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
  int* sortedArr = malloc(sizeof(int) * arrSize);
  memcpy(sortedArr, inputArr, arrSize * sizeof(int));
  doQuickSort(sortedArr, 0, arrSize - 1);
  return sortedArr;
}
