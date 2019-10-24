#include"util.hpp"
#include"receiver.hpp"

char BufferedReader::start = 0b10101010;
char BufferedReader::end = 0b11001100;
char BufferedWriter::start = 0b10101010;
char BufferedWriter::end = 0b11001100;

int main(int argc, char **argv)
{
	// Put your covert channel setup code here

	printf("Please press enter.\n");

	char text_buf[2];
	fgets(text_buf, sizeof(text_buf), stdin);

	printf("Receiver now listening.\n");

	bool listening = true;
	while (listening) {

		// Put your covert channel code here

	}

	printf("Receiver finished.\n");

	return 0;
}
