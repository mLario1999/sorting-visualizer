#pragma once


#define UNKNOWN_SORT 0
#define INSERTION_SORT 1
#define BUBBLE_SORT 2

struct BubbleSortState
{
    int currentOuter;
    int currentInner;   
};

struct InsertionSortState
{   
    bool sorted; 
    int step;
    int j;
    int key;
    bool shifting;
};

struct SortState
{
    int type;
    union 
    {
        InsertionSortState insertion;
        BubbleSortState bubble;
    };
    bool sorted;
    int swappedLeft;
    int swappedRight;
};



void StepSort(SortState* state, int array[], int size);
void StepBubbleSort(SortState* state, int array[], int size);
void StepInsertionSort(SortState *state, int array[], int size);
void ResolveSortTypeFromArg(SortState* state, char* arg);