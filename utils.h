#pragma once

inline void swap(void* a, void* b, size_t size)
{
    char * ap = (char*)a;
    char * bp = (char*)b; 
    char tmp;
    for (int i = 0; i < size; i++)
    {
        tmp = ap[i];
        ap[i] = bp[i];
        bp[i] = tmp;
    }
}