
struct runway {
	int status;
	time_t last_time;
};
typedef struct runway * prunway;

prunway create_new_runway (void) {
	prunway way;
	way = (prunway)malloc(sizeof(struct runway));
	if (way != NULL) {
		way->status = 0;
		way->last_time = -1;
		return way;
	}
	printf("Out of Space!\n");
	return NULL;
}
