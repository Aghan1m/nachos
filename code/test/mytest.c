#include "syscall.h"

#define N 15

int A[N][N];

int
main() {
    //int i;
    //int result[N];
    //result[0] = 0;
    //result[1] = 1;
    //for (i = 2; i < N; i++)
    //{
    //    result[i] = result[i-1] + result[i-2];
    //}
    //Exit(result[N-1]);

	// test1
//	int i,j;
//	for(i=0, j=9999; i<N; i++) {
//		i += 5;
//		j &= i;
//	}
//	Halt();

	int i;
	for(i=0; i<N; i++) {
		A[i][i] = i;
	}

	Halt();
}
