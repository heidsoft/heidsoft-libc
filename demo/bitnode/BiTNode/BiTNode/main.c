//
//  main.c
//  BiTNode
//
//  Created by Jake liu on 2022/2/20.
//

#include <stdio.h>
#include <stdlib.h>
/**
 二叉树的二叉链存储表示
 */
typedef struct BitNode{
    char data;
    struct BitNode *lchild,*rchild; /*左右孩子指针*/
}BiTNode,*BiTree;

/**
 建立一颗空二叉树
 */
int Initiate(BiTree bt)
{
    //初始化建立二叉树bt的头节点，0 表示建立失败，1表示建立成功
    if((bt = (BiTNode *)malloc(sizeof(BiTNode))) == NULL)
        return 0;
    bt->lchild = NULL;
    bt->rchild = NULL;
    return -1;
}


/**
 生成一颗二叉树
 */
BiTree Create(char x, BiTree lbt, BiTree rbt)
{
    // 生成一颗以x为节点的数据域，以lbt  rbt 为左右子树的二叉树
    BiTree p;
    if((p = (BiTNode *)malloc(sizeof(BiTNode))) == NULL)
        return NULL;
    p->data = x;
    p->lchild = lbt;
    p->rchild = rbt;
    
    return p;
}

int main(int argc, const char * argv[]) {
    // insert code here...
    printf("Hello, World!\n");
    return 0;
}
