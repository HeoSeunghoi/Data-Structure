#include<stdio.h>
#include<string.h>
#define max_string_size 100
#define max_pattern_size 100

int pmatch();
void fail();
int failure[max_pattern_size];
char string[max_string_size];
char pat[max_pattern_size];

int main()
{
	FILE* fp = fopen("kmp.txt", "r");
	int result;
	fgets(string,max_string_size,fp);
	fgets(pat, max_pattern_size, fp);
	fclose(fp);
	fail(pat);
	result = pmatch(string, pat);
	for (int i = 0; i < strlen(pat); i++)
		printf("%d  ",failure[i]);

	printf("%d\n",result);
	return 0;
}

int pmatch(char* string, char* pat)
{
	int i = 0, j = 0;
	int lens = strlen(string);
	int lenp = strlen(pat);

	while (i < lens && j<lenp)
	{
		if (string[i] == pat[j])
		{
			i++;
			j++;
		}
		else if (j == 0) i++;
		else j = failure[j - 1] + 1;
	}
	return ((j == lenp) ? (i - lenp) : -1);
}

void fail(char* pat)
{
	int i, n = strlen(pat);
	failure[0] = -1;
	for (int j = 1; j < n; j++)
	{
		i = failure[j - 1];
		while ((pat[j] != pat[i + 1]) && (i >= 0))
			i = failure[i];
		if (pat[j] == pat[i + 1])
			failure[j] = i + 1;
		else failure[j] = -1;
	}

}