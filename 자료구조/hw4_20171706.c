#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>

#define MAX_EXPR_SIZE 100

typedef enum
{
	lparen, rparen, plus, minus, times, divide, mod, eos, operand
}precedence;

typedef struct Head
{
	struct stack_list* next;
	precedence data;
}head;

typedef struct stack_list
{
	precedence data;
	struct stack_list* next;
}list;

struct Head* stack;
char expr[MAX_EXPR_SIZE];
int isp[] = { 0, 19, 12, 12, 13, 13, 13, 0 };
int icp[] = { 20, 19, 12, 12, 13, 13, 13, 0 };

void postfix();
precedence getToken(char* symbol, int* n);
void rExpr(FILE* fp, char* str);
void push(precedence x);
precedence pop();
void printToken(precedence x);

int main()
{
	stack = (head*)malloc(sizeof(head));
	stack->next = NULL;

	FILE* fp = fopen("expr.txt", "r");
	rExpr(fp, expr);
	fclose(fp);
	postfix();
	free(stack);
	return 0;
}
void rExpr(FILE* fp, char* str)
{
	int i = 0;
	while (!feof(fp))
	{
		fscanf(fp,"%c",&str[i]);
		i++;
	}
}

precedence getToken(char* symbol, int* n)
{
	*symbol = expr[(*n)++];
	switch (*symbol) {
		case '(': return lparen;
		case ')': return rparen;
		case '+': return plus;
		case '-': return minus;
		case '*': return divide;
		case '/': return times;
		case '%': return mod;
		case '\0': return eos;
		default : return operand;
	}
}

void push(precedence x)
{
	list* new = (list*)malloc(sizeof(list));
	new->data = x;
	new->next = stack->next;
	stack->next = new;
}

precedence pop()
{
	if (stack->next == NULL)
		return stack->data;
	precedence a;
	list* del = stack->next;
	a = del->data;

	stack->next = del->next;
	free(del);
	return a;
}

void postfix()
{
	char symbol;
	precedence token;
	int n = 0;
	stack->data = eos;

	for (token = getToken(&symbol, &n); token != eos; token = getToken(&symbol, &n))
	{
		if (symbol == ' ') continue;
		else if (token == operand)
			printf("%c ", symbol);
		else if (token == rparen)
		{
			if (stack->next == NULL)
				continue;
			while (stack->next->data != lparen)
				printToken(pop());
			pop();
		}
		else
		{
			while (stack->next != NULL && isp[stack->next->data] >= icp[token])
				printToken(pop());
			push(token);
		}
	}
	while ((token = pop()) != eos)
		printToken(token);
	printf("\n");
}

void printToken(precedence x)
{
	switch ((int)x) {
	case 0: return printf("");
	case 1: return printf("( ");
	case 2: return printf("+ ");
	case 3: return printf("- ");
	case 4: return printf("/ ");
	case 5: return printf("* ");
	case 6: return printf("% ");
	case 7: return printf(" ");
	default: return printf("Error!");
	}
}