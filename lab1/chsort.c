#include "chsort.h"

void __strtoupr(char * str, const int len)
{
    
    int i = 0;
    for (; i < len; ++i)
    {
        if ((int)str[i] >= 97 && (int)str[i] <= 122)
            str[i] = (char)((int)str[i] - 32);
    }
}

void __merge(char ** arr, int l, int m, int r)
{
    int i, j, k;
    int n1 = m - l + 1;
    int n2 = r - m;
        
    char * l_arr[n1];
    char * r_arr[n2];

    for (i = 0; i < n1; i++)
        l_arr[i] = arr[l + i];
    for (j = 0; j < n2; j++)
        r_arr[j] = arr[m + 1 + j];

    i = 0;
    j = 0;
    k = l;

    while (i < n1 && j < n2) 
    {
        char * lcpy = (char*)malloc(strlen(l_arr[i]) + sizeof(char));
        char * rcpy = (char*)malloc(strlen(r_arr[j]) + sizeof(char));

        strcpy(lcpy, l_arr[i]);
        strcpy(rcpy, r_arr[j]);

        __strtoupr(lcpy, strlen(lcpy));
        __strtoupr(rcpy, strlen(rcpy));

        if (strcmp(lcpy, rcpy) <= 0) 
        {
            arr[k] = l_arr[i];
            i++;
        }
        else 
        {
            arr[k] = r_arr[j];
            j++;
        }
        k++;

        free(lcpy);
        free(rcpy);

    }

    while (i < n1) 
    {
        arr[k] = l_arr[i];
        i++;
        k++;
    }

    while (j < n2) 
    {
        arr[k] = r_arr[j];
        j++;
        k++;
    }
}

// a simple mergesort implementation
void __merge_sort(char ** arr, int l, int r)
{
    if (l < r) 
    {
        int mid = l + (r - l) / 2;

        __merge_sort(arr, l, mid);
        __merge_sort(arr, mid + 1, r);

        __merge(arr, l, mid, r);
    }
}

void sort_entries(char ** entries, const int num_of_entries)
{
    __merge_sort(entries, 0, num_of_entries - 1);
}