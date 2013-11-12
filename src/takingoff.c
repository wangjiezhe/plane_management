#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <signal.h>
#include "takeoff.h"
#include "data.h"

void handler() {
	prw->status = 0;
}

int takingoff (prunway prw) {
	signal(SIGALRM, handler);
	if (difftime(time(NULL), prw->offtime) >= t_u) {
		printf("The runway is ready!\n");
		return 0;
	}
	else {
		printf("The runway is being prepared, please wait...\n");
		alarm(t_u - difftime(time(NULL), prw->offtime));
		pause();
		printf("The runway is ready!\n");
	}
}

