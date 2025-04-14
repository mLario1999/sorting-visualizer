#include "stepwise_sorting.h"
#include "core.h"
#include "utils.h"

void ResolveSortTypeFromArg(SortState* state, char* arg)
{
    memset(state, 0, sizeof(SortState));
    if (strcmp(arg, "InsertionSort") == 0)
    {
        state->type = INSERTION_SORT;
        return;
    }
    if (strcmp(arg, "BubbleSort") == 0)
    {
        state->type = BUBBLE_SORT;
        return;
    }
    state->type = UNKNOWN_SORT;
}

void StepBubbleSort(SortState* state, int array[], int size)
{
    BubbleSortState* innerState = &state->bubble;
     assert(state->type == BUBBLE_SORT);
     if (!state->sorted)
        {
            state->swappedLeft = -1;
            state->swappedRight = -1;
            if (innerState->currentInner < size - innerState->currentOuter - 1)
            {
                if (array[innerState->currentInner] > array[innerState->currentInner+1])
                {
                    swap(&array[innerState->currentInner], &array[innerState->currentInner+1], sizeof(int));
                    state->swappedLeft = innerState->currentInner;
                    state->swappedRight = innerState->currentInner + 1;
                }
                innerState->currentInner++;
            }
            else 
            {
                innerState->currentInner = 0;
                innerState->currentOuter++;
            }
            if (innerState->currentOuter >= size-1)
            {
                    state->sorted = true;
            }
        }  
}


void StepInsertionSort(SortState *state, int array[], int size)
{
    assert(state->type == INSERTION_SORT);
    InsertionSortState* innerState = &state->insertion;
    if (!state->sorted)
    {
        state->swappedLeft = -1;
        state->swappedRight = -1;
        if (innerState->step == 0)
        {
            innerState->step++;
        }
        if (!innerState->shifting)
        {
            innerState->key = array[innerState->step];
            innerState->j = innerState->step - 1;
            innerState->shifting = true;
        }
        if (innerState->j>=0 && innerState->key < array[innerState->j])
        {
            state->swappedLeft = innerState->j;
            state->swappedRight = innerState->j + 1;
            array[innerState->j+1] = array[innerState->j];
            innerState->j--;
        }
        else
        {
            array[innerState->j+1] = innerState->key;
            innerState->step++;
            innerState->shifting = false;
        }
        if (innerState->step >= size)
        {
            state->sorted = true;
        }
    }
}

void StepSort(SortState* state, int array[], int size)
{
    switch (state->type)
    {
    case INSERTION_SORT:
        StepInsertionSort(state, array, size);
        break;
    case BUBBLE_SORT:
        StepBubbleSort(state, array, size);
        break;
    default:
        break;
    }
}