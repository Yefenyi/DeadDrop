#include "util.hpp"

float avg(int *ar, int len) {
    int sum = 0;
    for (int i = 0; i < len; i++){
        sum += ar[i];
    }

    return float(sum) / float(len);
}

int main(int argc, char **argv)
{
    // int testLen = 1000000;
    int testLen = 1;
    char a = 'a';
    int buffSize = 32 * 1024 / 8;
    char recieveBuff[buffSize];
    int times[testLen];
    // int times2[testLen];
    int avgTimes[128 * 1024];
    int time;
    uintptr_t ptr;

    for (int i = 0; i < buffSize; i++) {
        ptr = uintptr_t(&recieveBuff[i]);
        if (!(ptr & 0b111111111111)) { 
            printf("addr is %p\n", (void *)ptr);
            // break;
        }
    }

    // return 0;

    // while(1) {
    for (int i = 0; i < testLen; i++) {
        times[i] = measure_one_block_access_time(ptr);
        // times2[i] = measure_one_block_access_time(ptr+128);
        // printf("time is %i\n", measure_one_block_access_time(ptr));
    }

    printf("avg real is %f\n", avg(times, testLen));
    // printf("avg other is %f\n", avg(times2, testLen));

    return 0;

    printf("Please press enter.\n");

    char text_buf[2];
    fgets(text_buf, sizeof(text_buf), stdin);

    printf("Receiver now listening.\n");

    bool listening = true;
    while (listening) {
        time = measure_one_block_access_time((long unsigned int)&a);
        // fprintf(stderr, "%i\n", time);
        // time = measure_one_block_access_time((long unsigned int)&b);
        // fprintf(stderr, "%i\n", time);
    }

    printf("Receiver finished.\n");

    return 0;
}


