#include"util.hpp"

int main(int argc, char **argv)
{
	int buffSize = 32 * 1024;
	char sendBuff[buffSize];
	uintptr_t writeTo[8];
	char *ptr;

	int idx = 0;
	for (int i = 0; i < buffSize; i++) {
		uintptr_t ptr = uintptr_t(&sendBuff[i]);
		if (!(ptr & 0b111111111111)) {
			printf("addr is %p\n", (void *)ptr);
			writeTo[idx] = ptr;
			idx++;
		}
	}

	// return 0;

	while(1) {
		*((int *)writeTo[0]) = 1;
		*((int *)writeTo[1]) = 1;
		*((int *)writeTo[2]) = 1;
		*((int *)writeTo[3]) = 1;
		*((int *)writeTo[4]) = 1;
		*((int *)writeTo[5]) = 1;
		*((int *)writeTo[6]) = 1;
		*((int *)writeTo[7]) = 1;
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