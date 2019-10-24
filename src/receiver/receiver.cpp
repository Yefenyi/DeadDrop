#include "util.hpp"
// #include <unistd.h>
// #include <iostream>
#include <stdio.h>

// using namespace std;

const int measureCt = 10000000;

const int setCt = 8;
ADDR_PTR cacheSet[setCt];

// if 1s are turning to 0s, increase.
// if 0s are turning to 1s, decrease.
// const float cutoff = 47.8;
const float cutoff = 467000000;

float avg(int *ar, int len) {
    int sum = 0;
    for (int i = 0; i < len; i++){
        sum += ar[i];
    }

    return float(sum) / float(len);
}

int ctr = 0;
// Measures a 1 or 0 by timing <measureCt> accesses. 
int doGetBit() {
    // printf("ctr is %i\n", ctr++);
    register CYCLES start, end, elapsed;
    int volatile tmp;
    int result;

    start = rdtsc();

    for (int i = 0; i < measureCt; i++) {
        // printf("ptr is %p\n", ptr);
        // tmp &= *(char*)ptr;
        tmp &= *(char*)cacheSet[0];
        tmp &= *(char*)cacheSet[1];
        tmp &= *(char*)cacheSet[2];
        tmp &= *(char*)cacheSet[3];
        tmp &= *(char*)cacheSet[4];
        tmp &= *(char*)cacheSet[5];
        tmp &= *(char*)cacheSet[6];
        tmp &= *(char*)cacheSet[7];
    }

    end = rdtsc();

    elapsed = end - start;
    // printf("elapsed is %llu\n", elapsed);

    // When we put contention on the cache set, receiver access
    // time *decreases*. Don't ask me why. One of life's many mysteries.
    if (elapsed > cutoff) {
        result = 0;
    } else {
        result = 1;
    }

    // printf("started at %llu, ended at %llu\n", start, end);

    return result;

    // int times[100000];
    // for (int i = 0; i < 100000; i++) {
    //     times[i] = measure_one_block_access_time(cacheSet[0]);
    // }
    // printf("avg time is %f\n", avg(times, 100000));
}

// Reads a 1 or 0, then waits for the period to end.
int getBit() {
    CYCLES start, end;

    // calculate start and end of cycle
    start = rdtsc();
    end = getCycleEnd(start);
    // end += 46500000;

    // printf("started %llx, ending %llx\n", start, end);

    //buffer a bit
    while (rdtsc() < start + 46500000) {}

    // read sent bit
    int result = doGetBit();

    // wait for cycle to end
    while (rdtsc() < end) {}

    return result;
}

int main(int argc, char **argv)
{
    // register CYCLES start = rdtsc();
    // volatile int tmp = tmp * tmp;
    // register CYCLES end = rdtsc();
    // printf("start: %llu\n", start);
    // printf("end: %llu\n", end);
    // printf("elapsed: %llu\n", end-start);
    // return 0;

    int buffSize = 32 * 1024;
    char recieveBuff[buffSize];
    ADDR_PTR ptr;

    int idx = 0;
	for (int i = 0; i < buffSize; i++) {
		ptr = ADDR_PTR(&recieveBuff[i]);
		if (!(ptr & 0b111111111111)) {
			printf("addr is %p\n", (void *)ptr);
			cacheSet[idx++] = ptr;
			// break;
		}
	}

    // end = rdtsc();
    int oldBit = 0;
    while(1) {
        int bit = getBit();
        printf("Received a %i", bit);
        if (bit == oldBit)
            printf(" - bad!");
        printf("\n");
        oldBit = bit;
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


