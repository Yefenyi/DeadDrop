#include "util.hpp"
// #include <unistd.h>
// #include <iostream>
#include <stdio.h>

// using namespace std;

float avg(int *ar, int len) {
    int sum = 0;
    for (int i = 0; i < len; i++){
        sum += ar[i];
    }

    return float(sum) / float(len);
}

int main(int argc, char **argv)
{
	register int start, end;
	volatile int tmp;

	const int cyclePeriod = 5000;
	const int measureCt = 100000;

    int testLen = 1000000;
    // int testLen = 1;
    int buffSize = 32 * 1024;
    char recieveBuff[buffSize];
    int times[measureCt];
    // int times2[testLen];
    // int time;
    ADDR_PTR ptr;
	// const int setCt = 8;
	// ADDR_PTR cacheSet[setCt];

	for (int i = 0; i < buffSize; i++) {
		ptr = ADDR_PTR(&recieveBuff[i]);
		if (!(ptr & 0b111111111111)) {
			printf("addr is %p\n", (void *)ptr);
			// cacheSet[idx] = ptr;
			// idx++;
			break;
		}
	}

    end = rdtsc();
    while(1) {
    	// start = rdtsc();

    	// for (int i = 0; i < measureCt; i++) {
    	// 	tmp = *(int*)ptr;
    	// }

    	// end = rdtsc();
    	// printf("avg time is %f\n", (end - start) / float(measureCt));

    	for (int i = 0; i < measureCt; i++) {
    		times[i] = measure_one_block_access_time(ptr);
    	}
    	printf("avg time is %f\n", avg(times, measureCt));
	}

    return 0;

    // printf("Please press enter.\n");

    // char text_buf[2];
    // fgets(text_buf, sizeof(text_buf), stdin);

    // printf("Receiver now listening.\n");

    // bool listening = true;
    // while (listening) {
    //     time = measure_one_block_access_time((long unsigned int)&a);
    //     // fprintf(stderr, "%i\n", time);
    //     // time = measure_one_block_access_time((long unsigned int)&b);
    //     // fprintf(stderr, "%i\n", time);
    // }

    // printf("Receiver finished.\n");

    // return 0;
}


