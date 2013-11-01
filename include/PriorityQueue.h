#ifndef PRIORITY_QUEUE_H_
#define PRIORITY_QUEUE_H_ 1

#include <stdio.h>
typedef long DataType_pq

struct PriorityQueue {	/* 优先队列 */
	int MAXNUM;
	int n;
	DataType_pq * pq;	/* 堆中元素的顺序表示 */
};
typedef struct PriorityQueue * PPriorityQueue;

void add_heap (PPriorityQueue papq, DataType_pq x) {	/* 向优先队列中插入x，保持堆序性（小根堆） */
	int i;
	if (papq->n >= papg->MAXNUM) {
		printf("Full!\n");
		return ;
	}
	for (i = papg->n; i > 0 && papq->pq[(i-1)/2] > x; i = (i-1)/2)
		papq->pq[i] = papq->pq[(i-1)/2];	/* 空位向根移动，找到插入的位置 */
	papq->pq[i] = x;	/* 将x插入 */
	papq->n ++;
}

int isEmpty_heap (PPriorityQueue papq) {
	return (papq->n == 0);
}

void removeMin_heap (PPriorityQueue papq) {
	int s, i, child;
	DataType_pq temp;
	if (isEmpty_heap(papq)) {
		printf("Empty!\n");
		return ;
	}
	s = --papq->n;		/* 先删除 */
	temp = papq->pq[s];		/* 指向最后一个元素 */
	i = 0;		/* 空位 */
	child = 1;		/* 空位的左子节点 */
	while (child < s) {		/* 向下移动空位，找到最后元素存放的位置 */
		if (child < s-1 && papq->pq[child] > papq->pq[child+1])
			child++;	/* 选择比较小的子节点 */
		if (temp > papq->pq[child]) {
			papq->pq[i] = papq->[child];
			i = child;		/* 空位移到子节点 */
			child = 2*i+1;		/* 已到新空位的左子节点 */
		}
		else
			break;
	}
	papq->pq[i] = temp;
}

#endif	/* PriorityQueue.h */
