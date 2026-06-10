#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void generateRandomArray(int *arr, int size, int min, int max);
void printArray(int *arr, int size);
void swap(int *a, int *b);
long long getCurrentTimeMs();

#endif