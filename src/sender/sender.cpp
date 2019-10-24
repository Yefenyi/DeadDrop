#include"util.hpp"

const int buffSize = 32 * 1024;
volatile char sendBuff[buffSize];
const int setCt = 8;
ADDR_PTR cacheSet[setCt];

void busy_wait() {
	for (int i = 0; i < 999999999; i++) {};
}

int ctr = 0;
void sendBit(char bit) {
	// printf("ctr is %i\n", ctr++);
	CYCLES start, end, diff;

	start = rdtsc();
	end = getCycleEnd(start);

    // printf("started %llx, ending %llx\n", start, end);

	// diff = 1;
	// diff <<= 33;
	// end = start+diff;

    // printf("   start is %llx\n", start);
    // printf("must end b4 %llx\n", start+60000);
    // printf("     end is %llx.\n", end);

	// printf("started at %llu, ended at %llu\n", start, end);

	if (bit) {
		while (rdtsc() < end) {
			// write many times between calls to rdtsc,
			// to make the channel more consistent
			for(int i = 0; i < 100000; i++) {
				*((char *)cacheSet[0]) = 0;
				*((char *)cacheSet[1]) = 0;
				*((char *)cacheSet[2]) = 0;
				*((char *)cacheSet[3]) = 0;
				*((char *)cacheSet[4]) = 0;
				*((char *)cacheSet[5]) = 0;
				*((char *)cacheSet[6]) = 0;
				*((char *)cacheSet[7]) = 0;
			}
		}
	} else {
		while (rdtsc() < end) {}
	}
}

void findCacheSet() {
	ADDR_PTR ptr;
	int idx = 0;
	for (int i = 0; i < buffSize; i++) {
		ptr = ADDR_PTR(&sendBuff[i]);
		if (!(ptr & 0b111111111111)) {
			printf("addr %i is %p\n", idx, (void *)ptr);
			cacheSet[idx++] = ptr;
		}
	}
}

int main(int argc, char **argv)
{
	// CYCLES oldEnd, end, start = 29934043004928;
	// while(1) {
	// 	end = getCycleEnd(start);
	// 	if (end != oldEnd)
	// 		printf("in %llx out %llx\n", start, getCycleEnd(start));
	// 	oldEnd = end;
	// 	start += 1;
	// }

	findCacheSet();

	while(1) {
		printf("sending 1\n");
		sendBit(1);
		printf("sending 0\n");
		sendBit(0);
	}

	printf("Please type a message.\n");

	bool sending = true;
	while (sending) {
		// char text_buf[128];
		// fgets(text_buf, sizeof(text_buf), stdin);
	
		// sendMessage(&text_buf);
	}

	printf("Sender finished.\n");

	return 0;
}