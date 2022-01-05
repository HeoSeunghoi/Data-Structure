#define _CRT_SECURE_NO_WARNINGS
#define MAX_TERMS 100

#include<stdio.h>
#include<stdlib.h>

typedef struct treeNode* treePtr;
typedef struct treeNode {
    treePtr lchild;
    int data;
    treePtr rchild;
}treeN;

void inorder(treePtr);
void preorder(treePtr);
void insert(treePtr*, int);
treePtr modifiedSearch(treePtr, int);

void main()
{
    int i, n, A[MAX_TERMS];
    FILE* fp = fopen("input.txt", "r");
    treePtr tree = NULL;
    fscanf(fp, "%d", &n);
    for (i = 0; i < n; i++) //A에 data 저장
        fscanf(fp, "%d", &A[i]);
    fclose(fp);
    for (i = 0; i < n; i++) //A의 값을 treePtr에 저장
        insert(&tree, A[i]);
    
    printf("Inorder : ");
    inorder(tree);
    printf("\n");

    printf("Preorder : ");
    preorder(tree);
    printf("\n");

    return 0;
}

void inorder(treePtr ptr) {
    if (ptr) {
        inorder(ptr->lchild);
        printf("%d ", ptr->data);
        inorder(ptr->rchild);
    }
}

void preorder(treePtr ptr) {
    if (ptr) {
        printf("%d ", ptr->data);
        preorder(ptr->lchild);
        preorder(ptr->rchild);
    }
}

void insert(treePtr* ptr, int x) {
    treePtr node, tmp = modifiedSearch(*ptr, x);
    if (tmp || !(*ptr)) {
        node = (treePtr)malloc(sizeof(treeN));
        node->data = x;
        node->lchild = node->rchild = NULL;
        if (*ptr) {
            if (x < tmp->data)
                tmp->lchild = node;
            else
                tmp->rchild = node;
        }
        else
            *ptr = node;
    }
}

treePtr modifiedSearch(treePtr ptr, int x) {
    treePtr move = ptr;

    if (move == NULL)
        return NULL;
    if (move->data == x)
        return NULL;
    else if (move->data < x) {
        if (move->rchild == NULL)
            return move;
        else
            return modifiedSearch(move->rchild, x);
    }
    else if (move->data > x) {
        if (move->lchild == NULL)
            return move;
        else
            return modifiedSearch(move->lchild, x);
    }

}