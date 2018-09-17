#include <stdlib.h>
#include <windows.h>
#include <process.h>
#include <limits.h>

typedef struct _FAKE_STRUCT {
	long long p1;
	long long p2;
	long long p3;
} FAKE_STRUCT, *PFAKE_STRUCT;

PFAKE_STRUCT p = NULL;
PFAKE_STRUCT p1 = NULL;
PFAKE_STRUCT p2 = NULL;
PFAKE_STRUCT p3 = NULL;

BOOL init_fake_struct(long long p1, long long p2, long long p3, PFAKE_STRUCT *p) {
	PFAKE_STRUCT s = malloc(sizeof(PFAKE_STRUCT));
	if (s == NULL) {
		printf("[FATAL] Fail to allocate memory for PFAKE_STRUCT\n");
		return FALSE;
	}
	*p = s;
	s->p1 = p1;
	s->p2 = p2;
	s->p3 = p3;
	return TRUE;
}

void init_all_point() {
	printf("Allocate p1 with code: %d\n", init_fake_struct(100, 200, 300, &p1));
	printf("Allocate p2 with code: %d\n", init_fake_struct(9632475, 2425686321475, 1111111111111111, &p2));
	printf("Allocate p3 with code: %d\n", init_fake_struct(LLONG_MAX, 21576, LLONG_MIN, &p3));
	p = p1;
}

unsigned __stdcall read_val(void) {
	while (TRUE) {
		PFAKE_STRUCT ps = p;
		if (ps->p1 == 100 && ps->p2== 200 && ps->p3 == 300) {
			printf("[INFO] read p1\n");
		} else if (ps->p1 == 9632475 && ps->p2 == 2425686321475 && ps->p3 == 1111111111111111) {
			printf("[INFO] read p2\n");
		} else if (ps->p1 == LLONG_MAX && ps->p2 == 21576 && ps->p3 == LLONG_MIN) {
			printf("[INFO] read p3\n");
		} else {
			printf("[FATAL] We need a memory barrier\n");
			break;
		}
	}
	return 0;
}

unsigned __stdcall write_val(void) {
	while (TRUE) {
		int n = rand() % 3;
		if (n == 0) {
			p = p1;
		} else if (n == 1) {
			p = p2;
		} else {
			p = p3;
		}
	}
	return 0;
}

int main(void) {
	init_all_point();
	unsigned int threadId;
	HANDLE thread = _beginthreadex(NULL, 0, read_val, NULL, 0, &threadId);
	printf("Thread id: %d for reading val.\n", threadId);
	thread = _beginthreadex(NULL, 0, write_val, NULL, 0, &threadId);
	printf("Thread id: %d for writing val.\n", threadId);
	while (TRUE) {
		getchar();
	}
}