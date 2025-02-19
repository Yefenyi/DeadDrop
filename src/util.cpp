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
	const int x = 25;

	// add to ensure ending time is above the next threshold
    end = start + (1 << x) - 1;

	// zero out last bits to round back down
    end >>= x-1;
    end <<= x-1; 

    // printf("  start is %llx\n", start);
    // printf("min end is %llx\n", start+300000);
    // printf("    end is %llx\n", end);
    // if (start + 300000 < end) {
    //     printf("good!\n");
    // } else {
    //     printf("bad!\n");
    // }

    return end;
}

ADDR_PTR** getCacheSets() {
	char *cacheBuff = new char[cacheSize];
	ADDR_PTR** cacheSets = new ADDR_PTR*[channels];

	// Cache set determined by last 12 bits
	ADDR_PTR ptr, mask;
	for (int channel = 0; channel < channels; channel++) {
		// mask =  (1<<12)-1
		cacheSets[channel] = new ADDR_PTR[ways];
		int idx = 0;
		for (int i = 0; i < cacheSize; i++) {
			ptr = ADDR_PTR(&cacheBuff[i]);
			// if last 6 bits are 0
			if ((ptr == (ptr >> 6) << 6) && ((ptr >> 6) & 0b111111) == channel) {
			// if (!(ptr & mask)) {
				printf("Set %i addr %i is %p\n", channel, idx, (void *)ptr);
				cacheSets[channel][idx++] = ptr;
			}
		}
	}

	return cacheSets;
}

// Measures a 1 or 0 by timing <measureCt> accesses. 
bool BufferedReader::doGetBit() {
    register CYCLES start, end;
    int volatile tmp;
    int result;
    int results[channels];

    start = rdtsc();

    for (int i = 0; i < measureCt; i++) {
    	int channel = 0;
    	for (int channel = 0; channel < channels; channel++){
	        tmp &= *(char*)cacheSets[channel][0];
	        tmp &= *(char*)cacheSets[channel][1];
	        tmp &= *(char*)cacheSets[channel][2];
	        tmp &= *(char*)cacheSets[channel][3];
	        tmp &= *(char*)cacheSets[channel][4];
	        tmp &= *(char*)cacheSets[channel][5];
	        tmp &= *(char*)cacheSets[channel][6];
	        tmp &= *(char*)cacheSets[channel][7];
	    }
    }

    end = rdtsc();

    delay = end - start;
    // printf("started at %llx, ended at %llx\n", start, end);
    // printf("delay is %llx or %llu\n", delay, delay);

    if (delay > cutoff) {
        return 1;
    } else {
        return 0;
    }
}

// Reads a 1 or 0, then waits for the period to end.
bool BufferedReader::getBit() {
    CYCLES start, end;
    bool good = false;

    // calculate start and end of cycle
    start = rdtsc();
    end = getCycleEnd(start);

    // printf("started %llx, ending %llx\n", start, end);

    //wait for a while, to give time for sender to send
    while (rdtsc() < start + (cutoff >> 3)) {}

    // read sent bit
    int result = doGetBit();

    // printf("received %i in %lli\n", result, delay);

    // wait for cycle to end
    while (rdtsc() < end) {
    	good = 1;
    }

    if (!good) {
    	printf("you clipped the inner loop! shame!\n");
    }

    return result;
}

void BufferedWriter::write(){
	// write_one_bit(1);
    write_one_char(char(junk));
    for(int i=0; i<127; i++){
        write_one_char(char(startByte));
        write_one_char(text_buf_[i]);
        if (text_buf_[i] == '\n')
        	break;
        // write_one_bit(text_buf_[i]=='1');
    }
}

void BufferedWriter::write_one_char(char c){
	printf("sending %c\n", c);
    for(int offset=7; offset>=0; offset--){
        write_one_bit(c&(1<<offset));
        // while(1){write_one_bit(0);write_one_bit(1);}
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
			for(int i = 0; i < 1000; i++) {
				for (int channel = 0; channel < channels; channel++) {
					*((char *)cacheSets[channel][0]) = 0;
					*((char *)cacheSets[channel][1]) = 0;
					*((char *)cacheSets[channel][2]) = 0;
					*((char *)cacheSets[channel][3]) = 0;
					*((char *)cacheSets[channel][4]) = 0;
					*((char *)cacheSets[channel][5]) = 0;
					*((char *)cacheSets[channel][6]) = 0;
					*((char *)cacheSets[channel][7]) = 0;
				}
			}
		}
	} else {
		// printf("0\n");
		while (rdtsc() < end) {}
	}

    // printf("actually ended %llx\n", rdtsc());
}

void BufferedReader::read(bool b){
    buffer <<= 1;
    buffer |= b;
    if(!isReading && buffer==startByte){
        isReading = true;
    }
    else if(isReading){
        (++counter)%=8;
        if(counter==0){
        	isReading = false;
        	buffer &= ~(1<<7); // 8th bit is always 0. Eliminates 1/8th of our errors!
            cout<<buffer<<std::flush;
            buffer = 0;
        }
    }
}