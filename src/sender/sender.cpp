#include"util.hpp"
#include"sender.hpp"
#include <iostream>
#include <unistd.h>

int main(int argc, char **argv)
{
	// Put your covert channel setup code here

	printf("Please type a message.\n");

	bool sending = true;

	size_t ways = 1;
	size_t bufferSize = (1<<15) * ways;

	char* array  = (char*)malloc(bufferSize * sizeof(char));

	for(int i=0; i<bufferSize; i++){
		touch_address(ADDR_PTR(array+i));
	}


	size_t counter=0;
	size_t maxMiss = 0;

	while(true){
		counter = 0;
		for(int i=0; i<bufferSize; i+=(1<<11)){
			if(measure_one_block_access_time(ADDR_PTR(array+i)) > 200){
				counter++;
				//std::cout<<(void*)(array+i)<<std::endl;
			}
		}
		maxMiss = std::max(counter, maxMiss);
		std::cout<< counter << std::endl << "\t" << maxMiss<<std::endl;
	}
	/*
	while (sending) {
		char text_buf[128];
		fgets(text_buf, sizeof(text_buf), stdin);
	
		// Put your covert channel code here
	}

	printf("Sender finished.\n");
	*/

	return 0;
}


