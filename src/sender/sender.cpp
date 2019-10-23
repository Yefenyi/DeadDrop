#include"util.hpp"

void busy_wait() {
	for (int i = 0; i < 999999999; i++) {};
}

int main(int argc, char **argv)
{
	int buffSize = 32 * 1024;
	volatile char sendBuff[buffSize];
	int writeCt = 8;
	ADDR_PTR writeTo[writeCt];
	char *ptr;

	int idx = 0;
	for (int i = 0; i < buffSize; i++) {
		ADDR_PTR ptr = ADDR_PTR(&sendBuff[i]);
		if (!(ptr & 0b111111111111)) {
			printf("addr %i is %p\n", idx, (void *)ptr);
			writeTo[idx++] = ptr;
		}
	}

	while(1) {
		printf("conflict!\n");
		for(int i = 0; i < 70000000; i++) {
			*((char *)writeTo[0]) = 0;
			*((char *)writeTo[1]) = 0;
			*((char *)writeTo[2]) = 0;
			*((char *)writeTo[3]) = 0;
			*((char *)writeTo[4]) = 0;
			*((char *)writeTo[5]) = 0;
			*((char *)writeTo[6]) = 0;
			*((char *)writeTo[7]) = 0;
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
		for(int i = 0; i < 70000000*8; i++) {
			*((char *)writeTo[0]+65) = 0;
			// *((int *)writeTo[1]+128) = 4;
			// *((int *)writeTo[2]+128) = 4;
			// *((int *)writeTo[3]+128) = 4;
			// *((int *)writeTo[4]+128) = 4;
			// *((int *)writeTo[5]+128) = 4;
			// *((int *)writeTo[6]+128) = 4;
			// *((int *)writeTo[7]+128) = 4;
			// flush(writeTo[0]+65);
			// flush(writeTo[1]+128);
			// flush(writeTo[2]+128);
			// flush(writeTo[3]+128);
			// flush(writeTo[4]+128);
			// flush(writeTo[5]+128);
			// flush(writeTo[6]+128);
			// flush(writeTo[7]+128);
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