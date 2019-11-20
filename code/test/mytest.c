#include "syscall.h"

#define N 200

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
	int i,j;
	for(i=0, j=9999; i<N; i++) {
		i += 5;
		j &= i;
	}
	Halt();
}
