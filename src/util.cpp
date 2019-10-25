#include "util.hpp"

using namespace std;

CYCLES delay;

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

CYCLES rdtsc() {
    unsigned hi, lo;
    asm volatile(
		"lfence\n\t"
		"rdtsc\n\t"
		"lfence\n\t"
		: "=a"(lo), "=d"(hi));
    return ( (unsigned long long)lo)|( ((unsigned long long)hi)<<32 );
}

// round starting time up to the nearst 2^x
CYCLES getCycleEnd(CYCLES start) {
	CYCLES end;
	const int x = 28;

	// add to ensure ending time is above the next threshold
    end = start + (1 << x) - 1;

	// zero out last bits to round back down
    end >>= x;
    end <<= x; 

    // printf("  start is %llx\n", start);
    // printf("min end is %llx\n", start+463000000);
    // printf("    end is %llx\n", end);
    // if (start + 463000000 < end) {
    //     printf("good!\n");
    // } else {
    //     printf("bad!\n");
    // }

    return end;
}

ADDR_PTR* getCacheSet() {
	char *cacheBuff = new char[cacheSize];
	ADDR_PTR *cacheSet = new ADDR_PTR[ways];

	// Find addresses where last 12 bits == 0
	ADDR_PTR ptr, mask = (1<<12)-1;
	int idx = 0;
	for (int i = 0; i < cacheSize; i++) {
		ptr = ADDR_PTR(&cacheBuff[i]);
		if (!(ptr & mask)) {
			printf("Set addr %i is %p\n", idx, (void *)ptr);
			cacheSet[idx++] = ptr;
		}
	}

	return cacheSet;
}

// Measures a 1 or 0 by timing <measureCt> accesses. 
bool BufferedReader::doGetBit() {
    register CYCLES start, end;
    int volatile tmp;
    int result;

    start = rdtsc();

    for (int i = 0; i < measureCt; i++) {
        tmp &= *(char*)cacheSet[0];
        tmp &= *(char*)cacheSet[1];
        tmp &= *(char*)cacheSet[2];
        tmp &= *(char*)cacheSet[3];
        tmp &= *(char*)cacheSet[4];
        tmp &= *(char*)cacheSet[5];
        tmp &= *(char*)cacheSet[6];
        tmp &= *(char*)cacheSet[7];
    }

    end = rdtsc();

    delay = end - start;
    // printf("started at %llu, ended at %llu\n", start, end);
    // printf("delay is %llu\n", delay);

    // if (delay > high) {
    //     high = delay;
    //     printf("new high: %llu\n", high);
    // }
    // if (delay < low) {
    //     low = delay;
    //     printf("new low: %llu\n", low);
    // }

    if (delay > cutoff) {
        return 1;
    } else {
        return 0;
    }
}

// Reads a 1 or 0, then waits for the period to end.
bool BufferedReader::getBit() {
    CYCLES start, end;

    // calculate start and end of cycle
    start = rdtsc();
    end = getCycleEnd(start);

    // printf("started %llx, ending %llx\n", start, end);

    //wait for a while, to give time for sender to send
    while (rdtsc() < start + (cutoff * 0.1)) {}

    // read sent bit
    int result = doGetBit();

    printf("received %i in %lli\n", result, delay);

    // wait for cycle to end
    while (rdtsc() < end) {}

    return result;
}

void BufferedWriter::write(){
    write_one_char(char(endByte));
    write_one_char(char(startByte));
    for(int i=0; i<127; i++){
        if (text_buf_[i] == '\n')
        	break;
        write_one_char(text_buf_[i]);
        // write_one_bit(text_buf_[i]=='1');
    }
    write_one_char(char(endByte));
}

void BufferedWriter::write_one_char(char c){
	printf("sending %c\n", c);
    for(int offset=7; offset>=0; offset--){
        write_one_bit(c&(1<<offset));
    }
}

void BufferedWriter::write_one_bit(bool b){
	CYCLES start, end;

	start = rdtsc();
	end = getCycleEnd(start);

    // printf("started %llx, ending %llx\n", start, end);

	if (b) {
		// printf("1\n");
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
		// printf("0\n");
		while (rdtsc() < end) {}
	}
}

void BufferedReader::read(bool b){
    buffer<<=1;
    buffer|=b;
    if(!isReading && buffer == startByte){
        cout<<"started"<<endl;
        isReading=true;
    }
    else if(isReading){
        (++counter)%=8;
        if(counter==0){
            if(buffer == endByte){
                isReading = false;
                cout<<endl<<"ended";
            }
            else{
                cout<<buffer<<std::flush;
            }
        }
    }
}
