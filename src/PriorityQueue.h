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

PPriorityQueue create_heap (int m);

void add_heap (PPriorityQueue papq, DataType_pq x);	/* 向优先队列中插入x，保持堆序性（小根堆） */

int isEmpty_heap (PPriorityQueue papq);

void removeMin_heap (PPriorityQueue papq);

#endif	/* PriorityQueue.h */
