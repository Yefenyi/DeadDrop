
#include "util.hpp"

/* Measure the time it takes to access a block with virtual address addr. */
CYCLES measure_one_block_access_time(ADDR_PTR addr)
{
	CYCLES cycles;

	asm volatile("mov %1, %%r8\n\t"
	"lfence\n\t"
	"rdtsc\n\t"
	"mov %%eax, %%edi\n\t"
	"mov (%%r8), %%r8\n\t"
	"lfence\n\t"
	"rdtsc\n\t"
	"sub %%edi, %%eax\n\t"
	: "=a"(cycles) /*output*/
	: "r"(addr)
	: "r8", "edi");	

	return cycles;
}


void BufferedReader::read(bool b){
    buffer<<=1;
    buffer|=b;
    if(!isReading && buffer==BufferedReader::start){
        cout<<"started"<<endl;
        isReading=true;
    }
    else if(isReading){
        (++counter)%=8;
        if(counter==0){
            if(buffer==BufferedReader::end){
                isReading = false;
                cout<<endl<<"ended";
            }
            else{
                cout<<buffer;
            }
        }
    }
}

void BufferedWriter::write(){
    write_one_char(char(BufferedWriter::start));
    for(int i=0; i<127; i++){
        write_one_char(text_buf_[i]);
    }
    write_one_char(char(BufferedWriter::end));
}

void BufferedWriter::write_one_char(char c){
    for(int offset=7; offset>=0; offset--){
        write_one_bit(c|(1<<offset));
    }
}

void BufferedWriter::write_one_bit(bool b){
        /*
         your code goes here
         */
    }