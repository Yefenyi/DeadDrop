#include"util.hpp"
#include"receiver.hpp"
#include <stdio.h>

// CYCLES getBitRate() {
//     doGetBit();
//     printf("delay was %llu\n", delay);
// }

int main(int argc, char **argv)
{
    BufferedReader reader(getCacheSets());

    // getBitRate();

	printf("Please press enter.\n");

	char text_buf[2];
	fgets(text_buf, sizeof(text_buf), stdin);

	printf("Receiver now listening.\n");

	bool listening = true;
	while (listening) {
        reader.read(reader.getBit());
	}

    printf("Receiver finished.\n");

	return 0;
}
