#include<stdio.h>

#define MAX_SIZE 101

void padd(int startA, int finishA, int startB, int finishB, int* startD, int* finishD);

typedef struct Arr {
	int expon;
	float coef;
}arr;
arr* arrD;
int avail = 0;

int main()
{
	FILE* fp1 = fopen("A.txt", "r");
	FILE* fp2 = fopen("B.txt", "r");
	
	int i = 0;
	int starta, finisha, startb, finishb;
	int startd=0, finishd=0;
	arrD = (arr*)malloc(sizeof(arr) * MAX_SIZE);
	starta = avail;
	while (1)
	{
		if (!feof(fp1))
			fscanf(fp1, "%f%d", &arrD[avail].coef, &arrD[avail].expon);
		else break;
		avail++;
	}
	fclose(fp1);
	finisha = avail-1;
	startb = avail;
	while (1)
	{
		if (!feof(fp2))
			fscanf(fp2, "%f%d", &arrD[avail].coef, &arrD[avail].expon);
		else break;
		avail++;
	}
	fclose(fp2);
	finishb = avail-1;

	padd(starta, finisha, startb, finishb, &startd, &finishd);

	for (i = startd; i < finishd; i++)
		printf("(%.f, %d), ",arrD[i].coef,arrD[i].expon);
	printf("(%.f, %d)\n", arrD[i].coef, arrD[i].expon);

	return 0;
}


void padd(int startA, int finishA, int startB, int finishB, int* startD, int* finishD)
{
	*startD = avail;
	while (startA <= finishA && startB <= finishB)
	{
		if (arrD[startA].expon < arrD[startB].expon)
		{
			arrD[avail].expon = arrD[startB].expon;
			arrD[avail].coef = arrD[startB].coef;
			startB++;
			avail++;
		}
		else if (arrD[startA].expon == arrD[startB].expon)
		{
			if (arrD[startA].coef + arrD[startB].coef != 0)
			{
				arrD[avail].coef = arrD[startA].coef + arrD[startB].coef;
				arrD[avail].expon = arrD[startA].expon;
				avail++;
			}
			startA++;
			startB++;
		
		}
		else
		{
			arrD[avail].expon = arrD[startA].expon;
			arrD[avail].coef = arrD[startA].coef;
			startA++;
			avail++;
		}
	}
	for (; startA <= finishA; startA++)
	{
		arrD[avail].expon = arrD[startA].expon;
		arrD[avail].coef = arrD[startA].coef;
		avail++;
	}
	for (; startB <= finishB; startB++)
	{
		arrD[avail].expon = arrD[startB].expon;
		arrD[avail].coef = arrD[startB].coef;
		avail++;
	}
	*finishD = avail-1;
}