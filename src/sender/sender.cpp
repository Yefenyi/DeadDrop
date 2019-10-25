#include "util.hpp"
#include "sender.hpp"

int main(int argc, char **argv)
{
	ADDR_PTR** cacheSets = getCacheSets();

	printf("Please type a message.\n");

	bool sending = true;
	while (sending) {
		char text_buf[128];
		BufferedWriter writer(text_buf, cacheSets);
		fgets(text_buf, sizeof(text_buf), stdin);
		// if (text_buf[0] == '0') {
		// 	for (int i = 0; i < 10; i++)
		// 		writer.write_one_bit(0);
		// } else {
		// 	for (int i = 0; i < 10; i++)
		// 		writer.write_one_bit(1);
		// }
		writer.write();
		printf("sent.\n");
	}

	printf("Sender finished.\n");

	return 0;
}


