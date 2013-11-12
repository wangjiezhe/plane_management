struct runway {
	int status;
	time_t offtime;
};
typedef struct runway * prunway;

extern prunway prw;

extern void handler(NUll);
extern int takingoff(prunway prw);
