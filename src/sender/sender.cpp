#include"util.hpp"

typedef struct foo{
	long long first;
	long long second;
	long long third;
	long long fourth;
	long long fifth;
	long long sixth;
	long long seventh;
	long long eighth;
} struct64;

typedef struct bar{
	long long array[8];
} struct64_2;

void busy_wait() {
	for (int i = 0; i < 999999999; i++) {};
}

int main(int argc, char **argv)
{
	printf("%lu\n", sizeof(struct64));
	printf("%lu\n", sizeof(struct64_2));

	int buffSize = 32 * 1024;
	char sendBuff[buffSize];
	int writeCt = 8;
	ADDR_PTR writeTo[writeCt];
	char *ptr;

	int idx = 0;
	for (int i = 0; i < buffSize; i++) {
		uintptr_t ptr = uintptr_t(&sendBuff[i]);
		if (!(ptr & 0b111111111111) && idx < writeCt) {
			printf("addr %i is %p\n", idx, (void *)ptr);
			writeTo[idx] = ptr;
			idx++;
		}
	}

	while(1) {
		printf("conflict!\n");
		for(int i = 0; i < 300000000; i++) {
			*((int *)writeTo[0]) = 4;
			*((int *)writeTo[1]) = 4;
			*((int *)writeTo[2]) = 4;
			*((int *)writeTo[3]) = 4;
			*((int *)writeTo[4]) = 4;
			*((int *)writeTo[5]) = 4;
			*((int *)writeTo[6]) = 4;
			*((int *)writeTo[7]) = 4;
			// flush(writeTo[0]);
			// flush(writeTo[1]);
			// flush(writeTo[2]);
			// flush(writeTo[3]);
			// flush(writeTo[4]);
			// flush(writeTo[5]);
			// flush(writeTo[6]);
			// flush(writeTo[7]);
		}
		printf("no conflict.\n");
		for(int i = 0; i < 300000000; i++) {
			*((int *)writeTo[0]+64) = 4;
			*((int *)writeTo[1]+64) = 4;
			*((int *)writeTo[2]+64) = 4;
			*((int *)writeTo[3]+64) = 4;
			*((int *)writeTo[4]+64) = 4;
			*((int *)writeTo[5]+64) = 4;
			*((int *)writeTo[6]+64) = 4;
			*((int *)writeTo[7]+64) = 4;
			// flush(writeTo[0]+64);
			// flush(writeTo[1]+64);
			// flush(writeTo[2]+64);
			// flush(writeTo[3]+64);
			// flush(writeTo[4]+64);
			// flush(writeTo[5]+64);
			// flush(writeTo[6]+64);
			// flush(writeTo[7]+64);
		}		

		// printf("writing to all sets\n");
		// for(int i = 0; i < 30000; i++) {
		// 	for(int i = 0; i < buffSize; i++) {
		// 		sendBuff[i] = 4;
		// 	}
		// }	

		// printf("writing to no sets\n");
		// for(int i = 0; i < 30000; i++) {
		// 	for(int i = 0; i < buffSize; i++) {
		// 		volatile int tmp = i * i;
		// 	}
		// }
	}

	return 0;

	// printf("diff: %i\n", writeTo[20] - writeTo[19]);

	for (int j = 0; j < 2; j++) {
		for (int i = 0; i < writeCt; i++) {
			printf("idx %i: ", i);
			print_access_time(writeTo[i]);
			printf("idx %i: ", i);
			print_access_time(writeTo[i]);
			printf("idx %i: ", i);
			flush(writeTo[i]);
			print_access_time(writeTo[i]);
		}
		// busy_wait();
	}

	return 0;

	while(1) {
		*((char *)writeTo[0]) = 1;
		*((char *)writeTo[1]) = 1;
		*((char *)writeTo[2]) = 1;
		*((char *)writeTo[3]) = 1;
		*((char *)writeTo[4]) = 1;
		*((char *)writeTo[5]) = 1;
		*((char *)writeTo[6]) = 1;
		*((char *)writeTo[7]) = 1;
	}

	return 0;

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