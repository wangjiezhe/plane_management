#ifndef QUEUE_H_
#define QUEUE_H_ 1

struct SeqQueue{
	int MAXNUM;
	int f,r;
	DataType_q *q;
};
typedef struct SeqQueue * PSeqQueue;

PSeqQueue createEmptyQueue_seq (int m) {
	PSeqQueue paqu;
	paqu = (PSeqQueue)malloc(sizeof(struct SeqQueue));
	if (paqu != NULL) {
		paqu->q = (DataType_q*)malloc(m*sizeof(DataType_q));
		if (paqu->q) {
			paqu->MAXNUM = m;
			paqu->f = paqu->r = 0;
			return paqu;
		}
		else
			free(paqu);
	}
	printf("Out of space!\n");
	return NULL;
}

int isEmptyQueue_seq (PSeqQueue paqu) {
	return (paqu->f == paqu->r);
}

void enQueue_seq (PSeqQueue paqu, DataType_q x) {
	if ((paqu->r + 1) % paqu->MAXNUM == paqu->f)
		printf("Full queue!\n");
	else {
		paqu->q[paqu->r] = x;
		paqu->r = (paqu->r + 1) % paqu->MAXNUM;
	}
}

void deQueue_seq (PSeqQueue paqu) {
	if (paqu->f == paqu->r)
		printf("Empty queue!\n");
	else
		paqu->f = (paqu->f + 1) % paqu->MAXNUM;
}

DataType_q frontQueue_seq (PSeqQueue paqu) {
	if (paqu->f == paqu->r)
		printf("Empty queue!\n");
	else
		return (paqu->q[paqu->f]);
}


struct Node;
typedef struct Node * PNode;
struct Node {
	DataType_q info;
	PNode link;
};
struct LinkQueue {
	PNode f;
	PNode r;
};
typedef struct LinkQueue * PLinkQueue;

PLinkQueue createEmptyQueue_link (void) {
	PLinkQueue plqu;
	plqu = (PLinkQueue)malloc(sizeof(struct LinkQueue));
	if (plqu != NULL)
		plqu->f = plqu->r = NULL;
	else
		printf("Out of Space!\n");
	return plqu;
}

int isEmptyQueue_link (PLinkQueue plqu) {
	return (plqu->f == NULL);
}

void enQueue_link (PLinkQueue plqu, DataType_q x) {
	PNode p;
	p = (PNode)malloc(sizeof(struct Node));
	if (p == NULL)
		printf("Out of Space!\n");
	else {
		p->info = x;
		p->link = NULL;
		if (plqu->f == NULL)
			plqu->f = p;
		else
			plqu->r->link = p;
		plqu->r = p;
	}
}

void deQueue_link(PLinkQueue plqu, DataType_q x) {
	PNode p;
	if (plqu->f == NULL)
		printf("Empty queue!\n");
	else {
		p = plqu->f;
		plqu->f = p->link;
		free(p);
	}
}

DataType_q frontQueue_link(PLinkQueue plqu) {
	if (plqu->f == NULL)
		printf("Emmpty queue!\n");
	else
		return (plqu->f->info);
}

#endif	/* Queue.h */
