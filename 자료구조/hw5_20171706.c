#define _CRT_SECURE_NO_WARNINGS
#define Max_Size 50

#include<stdio.h>
#include<stdlib.h>

typedef enum {head, entry} tagfield;
typedef struct Entry* entry_node;
typedef struct matrix_node* matrix_pointer;
typedef struct Entry {
	int row;
	int col;
	int value;
	entry_node next;
}entrynode;

typedef struct matrix_node {
	matrix_pointer down;
	matrix_pointer right;
	tagfield tag;
	union {
		matrix_pointer next;
		entry_node entry;
	}u;
	matrix_pointer hdnode[Max_Size];
}matrix;

entry_node sparse(FILE* fp) 
{
	int num_rows, num_cols, i = 0;
	// row, col의 개수를 저장해주는 int type변수 생성
	// i는 matrix의 크기가 되면 반복문 탈출하기 위해 생성
	int row = 0, col = 0, value = 0, count = 0;
	// 현재 row, col, value값을 저장
	// count는 matrix에 저장된 data의 개수
	entry_node headentry = (entry_node)malloc(sizeof(entrynode));
	headentry->next = NULL;
	entry_node tmp;
	entry_node cur = headentry;
	// headentry는 sparse형태로 저장한 list의 head
	// tmp는 value가 0이 아닌 경우 저장해 줄 노드
	// cur는 headentry의 마지막 노드

	fscanf(fp, "%d %d", &num_rows, &num_cols);
	// input file에서 데이터를 읽어 row, col의 개수를 저장

	while (!feof(fp) && i < num_rows * num_cols) {
		fscanf(fp, "%d", &value);
		if (value != 0) {
			tmp = (entry_node)malloc(sizeof(entrynode));
			tmp->col = col;
			tmp->row = row;
			tmp->value = value;
			tmp->next = NULL;
			if (cur->next == NULL)
				cur->next = tmp;
			while (cur->next != NULL)
				cur = cur->next;
			count++;
		} // input file의 데이터를 모두 읽을 때까지 반복

		col++;
		i++;
		//input file에서 data를 읽을 때마다 col, i에 1씩 증가
		if (col == num_cols) {
			col = 0;
			row++;
		}
		// col이 열의 개수와 같아지면 다시 0으로 초기화해주고
		//row를 1 증가시킴
	}
	headentry->row = num_rows;
	headentry->col = num_cols;
	headentry->value = count;
	// headentry에 row, col, value의 개수를 저장

	return headentry;
} // input file에서 데이터를 읽고 sparse matrix 형태로 저장해서 return해주는 함수

matrix_pointer mread(entry_node headentry)
{
	int num_rows, num_cols, num_terms, num_heads, i;
	// row, col, value, head_node의 개수를 저장해주는 int type변수 생성
	// i는 head_node의 index
	int row, col, value, current_row;
	// entry에 저장시켜줄 row, col, value변수 생성
	// current_row는 entry_node에서 읽어오는 row값이
	//current_row보다 큰 경우
	// 현재 entry의 right를 현재 hdnode에 연결하기 위해 생성
	matrix_pointer temp;
	matrix_pointer last;
	matrix_pointer node;
	// tmp는 entry_node에서 읽어온 데이터를 저장해 hdnode에 연결시키기 위해 생성
	// last는 행과 열에 데이터를 연결시켜주는 위치를 저장
	// node는 head끼리 연결시키고 반환해 주기 위해 생성

	entry_node tmp = (entry_node)malloc(sizeof(entrynode));
	matrix_pointer* hdnode = (matrix_pointer)malloc(sizeof(matrix));

	num_rows = headentry->row;
	num_cols = headentry->col;
	num_terms = headentry->value;
	headentry = headentry->next;
	// row, col, value의 개수를 저장
	// head에 연결된 데이터를 읽어야하므로 head에 head->next로 바꿔줌

	num_heads = (num_cols > num_rows) ? num_cols : num_rows;
	// row와 col중에서 더 큰 값을 head의 개수로 저장
	node = (matrix_pointer)malloc(sizeof(matrix));
	node->u.entry = tmp;
	node->tag = entry;
	node->u.entry->row = num_rows;
	node->u.entry->col = num_cols;
	

	if (!num_heads)node->right = node;
	// matrix가 비어있을 경우 node에 node를 연결시켜주고 node를 반환
	else {
		for (i = 0; i < num_heads; i++) {
			temp = (matrix_pointer)malloc(sizeof(matrix));
			hdnode[i] = temp;
			hdnode[i]->tag = head;
			hdnode[i]->right = temp;
			hdnode[i]->u.next = temp;
		} // num_heads만큼 hdnode를 생성
		current_row = 0;
		last = hdnode[0];
		// 처음 연결시켜 줄 hdnode[0]을 last로 저장, row = 0에서
		// 시작하므로 current_row = 0
		for (i = 0; i < num_terms; i++) {

			if (headentry == NULL)
				printf("Error : head is NULL!\n");
			// head가 NULL인 경우 error
			row = headentry->row;
			col = headentry->col;
			value = headentry->value;
			headentry = headentry->next;
			// entry의 데이터를 row, col, value에 저장

			if (row > current_row) {
				last->right = hdnode[current_row];
				current_row = row;
				last = hdnode[row];
			}
			// row가 current_row보다 큰 경우 현재 행의 마지막 entry의
			// right를 hdnode에 연결
			
			temp = (matrix_pointer)malloc(sizeof(matrix));
			tmp = (entry_node)malloc(sizeof(entrynode));
			temp->u.entry = tmp;
			temp->tag = entry;
			temp->u.entry->row = row;
			temp->u.entry->col = col;
			temp->u.entry->value = value;
			// temp에 row, col, value의 값을 저장해준다

			last->right = temp;
			last = temp;
			// 현재 마지막 entry의 right에 temp를 저장해주고
			//last를 temp로 초기화
			hdnode[col]->u.next->down = temp;
			hdnode[col]->u.next = temp;
			// 새로 연결된 entry를 행에 대해서 연결
		} // 각 row와 col 방향으로 entry를 연결
		
		last->right = hdnode[current_row];
		//closeallcolumnlists
		for (i = 0; i < num_cols; i++)
			hdnode[i]->u.next->down = hdnode[i];
		//link all head nodes together
		for (i = 0; i < num_heads - 1; i++)
			hdnode[i]->u.next = hdnode[i + 1];
		hdnode[num_heads - 1]->u.next = node;
		node->right = hdnode[0];
	}
	return node;
} // file에서 matrix 자료를 읽어 리스트에 저장한다.
void mwrite(matrix_pointer node)
{
	int i;


	matrix_pointer temp, head = node->right;


	for (i = 0; i < node->u.entry->row; i++) {
		for (temp = head->right; temp != head; temp = temp->right)
			printf("(%d %d %d) ",temp->u.entry->row, temp->u.entry->col,
				temp->u.entry->value);
		head = head->u.next;
	}

}// 행렬을 적절한 포맷 (2차원 행렬형태)으로 출력한다.


void merase(matrix_pointer* node)
{
	int i, num_heads;
	matrix_pointer x, y;
	matrix_pointer head = (*node)->right;
	for (i = 0; i < (*node)->u.entry->row; i++) {
		y = head->right;
		while (y != head) {
			x = y;
			y = y->right;
			free(x);
		}
		x = head;
		head = head->u.next;
		free(x);
	}
	
	y = head;
	while (y != *node) {
		x = y;
		y = y->u.next;
		free(x);
	}
	free(*node);
	*node = NULL;

}// NULL parameter인 경우 적절히 처리.

int main()
{
	entry_node headentry_a = (entry_node)malloc(sizeof(entrynode));
	entry_node headentry_b = (entry_node)malloc(sizeof(entrynode));

	matrix_pointer head_a = (matrix_pointer)malloc(sizeof(matrix));
	matrix_pointer head_b = (matrix_pointer)malloc(sizeof(matrix));

	FILE* fp_a = fopen("A.txt", "r");
	FILE* fp_b = fopen("B.txt", "r");
	headentry_a = sparse(fp_a);
	headentry_b = sparse(fp_b);
	head_a = mread(headentry_a);
	head_b = mread(headentry_b);
	mwrite(head_a);
	printf("\n");
	mwrite(head_b);
	merase(&head_a);
	merase(&head_b);

	return 0;
}