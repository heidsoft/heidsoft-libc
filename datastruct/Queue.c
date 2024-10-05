#include <stdio.h>
#include <stdlib.h>

#define MAX_SIZE 10

// 定义队列结构体
typedef struct {
    int items[MAX_SIZE];
    int front;
    int rear;
} Queue;

// 创建队列
Queue *createQueue() {
    Queue *queue = (Queue *)malloc(sizeof(Queue));
    if (queue == NULL) {
        printf("内存分配失败\n");
        exit(1);
    }
    queue->front = -1;
    queue->rear = -1;
    return queue;
}

// 销毁队列
void destroyQueue(Queue *queue) {
    if (queue == NULL) {
        return;
    }
    free(queue);
}

// 清空队列
void clearQueue(Queue *queue) {
    if (queue == NULL) {
        return;
    }
    queue->front = -1;
    queue->rear = -1;
}

// 检查队列是否为空
int isEmpty(Queue *queue) {
    if (queue == NULL) {
        return 1;
    }
    return queue->front == -1;
}

// 检查队列是否已满
int isFull(Queue *queue) {
    if (queue == NULL) {
        return 1;
    }
    return queue->rear == MAX_SIZE - 1;
}

// 入队
void enqueue(Queue *queue, int value) {
    if (queue == NULL || isFull(queue)) {
        printf("队列已满，无法入队\n");
        return;
    }
    if (isEmpty(queue)) {
        queue->front = 0;
    }
    queue->rear++;
    queue->items[queue->rear] = value;
}

// 出队
int dequeue(Queue *queue) {
    if (queue == NULL || isEmpty(queue)) {
        printf("队列空，无法出队\n");
        return -1;
    }
    int value = queue->items[queue->front];
    queue->front++;
    if (queue->front > queue->rear) {
        queue->front = queue->rear = -1;
    }
    return value;
}

// 获取队列元素个数
int size(Queue *queue) {
    if (queue == NULL) {
        return -1;
    }
    return queue->rear - queue->front + 1;
}

// 获取队列前端元素
int front(Queue *queue) {
    if (queue == NULL || isEmpty(queue)) {
        printf("队列空，无法获取队列前端元素\n");
        return -1;
    }
    return queue->items[queue->front];
}

// 打印队列元素
void printQueue(Queue *queue) {
    if (queue == NULL) {
        printf("队列空，无法打印\n");
        return;
    }
    int i = queue->front;
    printf("队列元素：");
    while (i <= queue->rear) {
        printf("%d ", queue->items[i]);
        i++;
    }
    printf("\n");
}

int main() {
    Queue *queue = createQueue();
    enqueue(queue, 1);
    enqueue(queue, 2);
    enqueue(queue, 3);
    enqueue(queue, 4);
    enqueue(queue, 5);

    printf("队列元素个数：%d\n", size(queue));
    printQueue(queue);

    dequeue(queue);
    dequeue(queue);

    printf("队列元素个数：%d\n", size(queue));
    printQueue(queue);

    destroyQueue(queue);

    return 0;
}
