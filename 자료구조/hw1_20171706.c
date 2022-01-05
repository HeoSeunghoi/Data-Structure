#include<stdio.h>
#include<stdlib.h>
#include<time.h>
void swap(int* arr,int i,int j);
int main()
{
    clock_t start;
    start = clock();

    int n, i, j, x;
    int* arr;
    double duration;

    FILE* fp = fopen("lab1.data", "r");
    fscanf(fp, "%d", &n);
    arr = (int*)malloc(sizeof(int) * n);

    i = 0;
    //Save in arr
    while (1)
    {
        if (feof(fp)) break;
        fscanf(fp, "%d", &arr[i]);
        i++;
    }

    fclose(fp);

    //Print arr
    printf("Input: %d", arr[0]);
    for (i = 1; i < n; i++)
    {
        printf(", %d", arr[i]);
    }
    printf("\n");

    //Sorted
    for (i = 0; i < n; i++)
    {
        for (j = i; j < n; j++)
            if (arr[i] > arr[j])
                swap(arr, i, j);
    }

    //Print sorted arr
    printf("Sorted: %d", arr[0]);
    for (i = 1; i < n; i++)
    {
        printf(", %d", arr[i]);
    }
    printf("\n");

    duration = ((double)(clock() - start)) / CLOCKS_PER_SEC;

    printf("%.8lf", duration);
    return 0;
}

void swap(int* arr,int i,int j)
{
    int x;
    x = arr[i];
    arr[i] = arr[j];
    arr[j] = x;
}