#ifndef KNAPSACK_H
#define KNAPSACK_H

typedef struct {
    int weight;
    int value;
} Item;

int bruteForceKnapsack(Item *items, int n, int capacity);
int dynamicProgrammingKnapsack(Item *items, int n, int capacity);
int greedyKnapsack(Item *items, int n, int capacity);
int backtrackingKnapsack(Item *items, int n, int capacity);

#endif