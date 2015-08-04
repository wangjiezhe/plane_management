#ifndef QUEUE_H_
#define QUEUE_H_ 0

struct ground {
	char flight_name[7];
	time_t landing_time;
};

typedef struct ground DataType_q;

struct SeqQueue{
	int MAXNUM;
	int f,r;
	DataType_q *q;
};
typedef struct SeqQueue * PSeqQueue;

PSeqQueue createEmptyQueue (int m);

int isEmptyQueue (PSeqQueue paqu);

int isFullQueue (PSeqQueue paqu);

int isOnlyOneInQueue (PSeqQueue paqu);

void enQueue (PSeqQueue paqu, DataType_q x);

void deQueue (PSeqQueue paqu);

DataType_q frontQueue (PSeqQueue paqu);

#endif	/* Queue.h */
