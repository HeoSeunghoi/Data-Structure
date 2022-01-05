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
	// row, col�� ������ �������ִ� int type���� ����
	// i�� matrix�� ũ�Ⱑ �Ǹ� �ݺ��� Ż���ϱ� ���� ����
	int row = 0, col = 0, value = 0, count = 0;
	// ���� row, col, value���� ����
	// count�� matrix�� ����� data�� ����
	entry_node headentry = (entry_node)malloc(sizeof(entrynode));
	headentry->next = NULL;
	entry_node tmp;
	entry_node cur = headentry;
	// headentry�� sparse���·� ������ list�� head
	// tmp�� value�� 0�� �ƴ� ��� ������ �� ���
	// cur�� headentry�� ������ ���

	fscanf(fp, "%d %d", &num_rows, &num_cols);
	// input file���� �����͸� �о� row, col�� ������ ����

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
		} // input file�� �����͸� ��� ���� ������ �ݺ�

		col++;
		i++;
		//input file���� data�� ���� ������ col, i�� 1�� ����
		if (col == num_cols) {
			col = 0;
			row++;
		}
		// col�� ���� ������ �������� �ٽ� 0���� �ʱ�ȭ���ְ�
		//row�� 1 ������Ŵ
	}
	headentry->row = num_rows;
	headentry->col = num_cols;
	headentry->value = count;
	// headentry�� row, col, value�� ������ ����

	return headentry;
} // input file���� �����͸� �а� sparse matrix ���·� �����ؼ� return���ִ� �Լ�

matrix_pointer mread(entry_node headentry)
{
	int num_rows, num_cols, num_terms, num_heads, i;
	// row, col, value, head_node�� ������ �������ִ� int type���� ����
	// i�� head_node�� index
	int row, col, value, current_row;
	// entry�� ��������� row, col, value���� ����
	// current_row�� entry_node���� �о���� row����
	//current_row���� ū ���
	// ���� entry�� right�� ���� hdnode�� �����ϱ� ���� ����
	matrix_pointer temp;
	matrix_pointer last;
	matrix_pointer node;
	// tmp�� entry_node���� �о�� �����͸� ������ hdnode�� �����Ű�� ���� ����
	// last�� ��� ���� �����͸� ��������ִ� ��ġ�� ����
	// node�� head���� �����Ű�� ��ȯ�� �ֱ� ���� ����

	entry_node tmp = (entry_node)malloc(sizeof(entrynode));
	matrix_pointer* hdnode = (matrix_pointer)malloc(sizeof(matrix));

	num_rows = headentry->row;
	num_cols = headentry->col;
	num_terms = headentry->value;
	headentry = headentry->next;
	// row, col, value�� ������ ����
	// head�� ����� �����͸� �о���ϹǷ� head�� head->next�� �ٲ���

	num_heads = (num_cols > num_rows) ? num_cols : num_rows;
	// row�� col�߿��� �� ū ���� head�� ������ ����
	node = (matrix_pointer)malloc(sizeof(matrix));
	node->u.entry = tmp;
	node->tag = entry;
	node->u.entry->row = num_rows;
	node->u.entry->col = num_cols;
	

	if (!num_heads)node->right = node;
	// matrix�� ������� ��� node�� node�� ��������ְ� node�� ��ȯ
	else {
		for (i = 0; i < num_heads; i++) {
			temp = (matrix_pointer)malloc(sizeof(matrix));
			hdnode[i] = temp;
			hdnode[i]->tag = head;
			hdnode[i]->right = temp;
			hdnode[i]->u.next = temp;
		} // num_heads��ŭ hdnode�� ����
		current_row = 0;
		last = hdnode[0];
		// ó�� ������� �� hdnode[0]�� last�� ����, row = 0����
		// �����ϹǷ� current_row = 0
		for (i = 0; i < num_terms; i++) {

			if (headentry == NULL)
				printf("Error : head is NULL!\n");
			// head�� NULL�� ��� error
			row = headentry->row;
			col = headentry->col;
			value = headentry->value;
			headentry = headentry->next;
			// entry�� �����͸� row, col, value�� ����

			if (row > current_row) {
				last->right = hdnode[current_row];
				current_row = row;
				last = hdnode[row];
			}
			// row�� current_row���� ū ��� ���� ���� ������ entry��
			// right�� hdnode�� ����
			
			temp = (matrix_pointer)malloc(sizeof(matrix));
			tmp = (entry_node)malloc(sizeof(entrynode));
			temp->u.entry = tmp;
			temp->tag = entry;
			temp->u.entry->row = row;
			temp->u.entry->col = col;
			temp->u.entry->value = value;
			// temp�� row, col, value�� ���� �������ش�

			last->right = temp;
			last = temp;
			// ���� ������ entry�� right�� temp�� �������ְ�
			//last�� temp�� �ʱ�ȭ
			hdnode[col]->u.next->down = temp;
			hdnode[col]->u.next = temp;
			// ���� ����� entry�� �࿡ ���ؼ� ����
		} // �� row�� col �������� entry�� ����
		
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
} // file���� matrix �ڷḦ �о� ����Ʈ�� �����Ѵ�.
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

}// ����� ������ ���� (2���� �������)���� ����Ѵ�.


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

}// NULL parameter�� ��� ������ ó��.

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