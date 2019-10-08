#include"util.hpp"
#include"sender.hpp"
#include <iostream>
#include <unistd.h>

int main(int argc, char **argv)
{
	// Put your covert channel setup code here

	printf("Please type a message.\n");

	bool sending = true;

	size_t bufferSize = 1<<20;

	char* array = (char*)malloc(bufferSize*sizeof(char));

	std::cout <<"allocated char array of "<< bufferSize <<std::endl;

	std::cout << "Cycles used to access array[" <<size_t((1<<0)-1) << "]: " << measure_one_block_access_time(ADDR_PTR(&(array[0]))) << std::endl;
	//std::cout << "Cycles used to access array[" <<size_t((1<<11)-1) << "]: " << measure_one_block_access_time(ADDR_PTR(&(array[(1<<11)-1]))) << std::endl;
	std::cout << "Cycles used to access array[" <<size_t((1<<12)-1) << "]: " << measure_one_block_access_time(ADDR_PTR(&(array[(1<<12)-1]))) << std::endl;

    for(int i=0; i<=20; i++){
    	ADDR_PTR addr = ADDR_PTR(&(array[(1<<i)-1]));

    	std::cout << "Cycles used to access array[" <<size_t((1<<i)-1) << "]: " << measure_one_block_access_time(addr) << std::endl;
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


