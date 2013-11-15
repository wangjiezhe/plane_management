#ifndef PRIORITY_QUEUE_H_
#define PRIORITY_QUEUE_H_ 0

struct flight {
	char flight_name[7];
	time_t landing_deadline;
};

typedef struct flight DataType_pq;

struct PriorityQueue {	/* 优先队列 */
	int MAXNUM;
	int n;
	DataType_pq * pq;	/* 堆中元素的顺序表示 */
};
typedef struct PriorityQueue * PPriorityQueue;

PPriorityQueue create_heap (int m) {
	PPriorityQueue papq;
	papq = (PPriorityQueue)malloc(sizeof(struct PriorityQueue));
	if (papq != NULL) {
		papq->pq = (DataType_pq *)malloc(m*sizeof(DataType_pq));
		if (papq->pq) {
			papq->MAXNUM = m;
			papq->n = 0;
			return papq;
		}
		else free(papq);
	}
	printf("Out of space!\n");
	return NULL;
}

void add_heap (PPriorityQueue papq, DataType_pq x) {	/* 向优先队列中插入x，保持堆序性（小根堆） */
	int i;
	if (papq->n >= papq->MAXNUM) {
		printf("Full!\n");
		return ;
	}
	for (i = papq->n; i > 0 && papq->pq[(i-1)/2].landing_deadline > x.landing_deadline; i = (i-1)/2)
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
		if (child < s-1 && papq->pq[child].landing_deadline > papq->pq[child+1].landing_deadline)
			child++;	/* 选择比较小的子节点 */
		if (temp.landing_deadline > papq->pq[child].landing_deadline) {
			papq->pq[i] = papq->pq[child];
			i = child;		/* 空位移到子节点 */
			child = 2*i+1;		/* 已到新空位的左子节点 */
		}
		else
			break;
	}
	papq->pq[i] = temp;
}

#endif	/* PriorityQueue.h */
