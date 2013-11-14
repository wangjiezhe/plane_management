#ifndef QUEUE_H_
#define QUEUE_H_

#include <stdio.h>
#include <stdlib.h>

struct ground {
	char flight_name[7];
	time_t landing_time;
};

typedef struct ground DataType_q;

struct SeqQueue{
	int MAXNUM;
	int f,r;
	DataType_q *q;
//	time_t last_in_time;
//	time_t last_out_time;
};
typedef struct SeqQueue * PSeqQueue;

PSeqQueue createEmptyQueue (int m) {
	PSeqQueue paqu;
	paqu = (PSeqQueue)malloc(sizeof(struct SeqQueue));
	if (paqu != NULL) {
		paqu->q = (DataType_q*)malloc(m*sizeof(DataType_q));
		if (paqu->q) {
			paqu->MAXNUM = m;
			paqu->f = paqu->r = 0;
//			paqu->last_in_time = paqu->last_out_time = -1;
			return paqu;
		}
		else
			free(paqu);
	}
	printf("Out of space!\n");
	return NULL;
}

int isEmptyQueue (PSeqQueue paqu) {
	return (paqu->f == paqu->r);
}

int isFullQueue (PSeqQueue paqu) {
	return ((paqu->r +1) % paqu->MAXNUM == paqu->f);
}

void enQueue (PSeqQueue paqu, DataType_q x) {
	if ((paqu->r + 1) % paqu->MAXNUM == paqu->f)
		printf("Full queue!\n");
	else {
		paqu->q[paqu->r] = x;
		paqu->r = (paqu->r + 1) % paqu->MAXNUM;
	}
}

void deQueue (PSeqQueue paqu) {
	if (paqu->f == paqu->r)
		printf("Empty queue!\n");
	else
		paqu->f = (paqu->f + 1) % paqu->MAXNUM;
}

DataType_q frontQueue (PSeqQueue paqu) {
	if (paqu->f == paqu->r) {
		printf("Empty queue!\n");
	}
	else
		return (paqu->q[paqu->f]);
}


#endif	/* Queue.h */
