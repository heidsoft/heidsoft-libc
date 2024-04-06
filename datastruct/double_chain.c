#include <stdio.h>
#include <stdlib.h>

// 定义双向链表节点
struct Node {
    int data;
    struct Node* next;
    struct Node* prev;
};

// 创建双向链表
struct Node* createList() {
    struct Node* head = NULL;
    struct Node* tail = NULL;

    int value;

    printf("请输入要添加的值（以-1 结束输入）：\n");
    while (1) {
        scanf("%d", &value);
        if (value == -1) {
            break;
        }

        struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
        if (newNode == NULL) {
            printf("内存分配失败\n");
            exit(1);
        }

        newNode->data = value;
        newNode->next = NULL;
        newNode->prev = NULL;

        if (head == NULL) {
            head = newNode;
            tail = newNode;
        } else {
            tail->next = newNode;
            newNode->prev = tail;
            tail = newNode;
        }
    }

    return head;
}

// 打印双向链表
void printList(struct Node* head) {
    struct Node* current = head;
    while (current!= NULL) {
        printf("%d ", current->data);
        current = current->next;
    }
    printf("\n");
}

// 删除链表
void deleteList(struct Node* head) {
    struct Node* current = head;
    while (current!= NULL) {
        struct Node* next = current->next;
        free(current);
        current = next;
    }
}

// 双向链表节点插入操作
void insert(struct Node** head_ref, int new_data) {
  struct Node* new_node = (struct Node*)malloc(sizeof(struct Node));
  struct Node *last = *head_ref;

  new_node->data  = new_data;
  new_node->next = NULL;

  if (*head_ref == NULL) {
    new_node->prev = NULL;
    *head_ref = new_node;
    return;
  }

  while (last->next!= NULL)
    last = last->next;

  last->next = new_node;
  new_node->prev = last;
}

// 双向链表搜索操作
int search(struct Node* head, int key) {
  struct Node* current = head;
  while (current!= NULL) {
    if (current->data == key)
      return 1;
    current = current->next;
  }
  return 0;
}

// 双向链表节点删除操作
void deleteNode(struct Node** head_ref, int key) {
  struct Node* temp = *head_ref;
  struct Node* prev = NULL;

  if (*head_ref == NULL) {
    printf("链表为空\n");
    return;
  }

  if ((*head_ref)->data == key) {
    *head_ref = (*head_ref)->next;
    free(temp);
    return;
  }

  while (temp!= NULL && temp->data!= key) {
    prev = temp;
    temp = temp->next;
  }

  if (temp == NULL) {
    printf("未找到要删除的节点\n");
    return;
  }

  prev->next = temp->next;
  free(temp);
}

// 双向链表
// 2024-04-06
// test
int main() {
    struct Node* head = createList();
    printList(head);

    // 在第 3 个位置插入节点
    insert(&head, 3);
    printList(head);

    // 删除第 2 个节点
    deleteNode(&head, 2);
    printList(head);

    // 搜索 4
    if (search(head, 4)) {
        printf("找到\n");
    } else {
        printf("未找到\n");
    }

    // 删除链表
    deleteList(head);

    return 0;
}
