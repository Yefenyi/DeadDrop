// You may only use fgets() to pull input from stdin
// You may use any print function to stdout to print 
// out chat messages
#include <stdio.h>

// You may use memory allocators and helper functions 
// (e.g., rand()).  You may not use system().
#include <stdlib.h>
#include <iostream>
#include <inttypes.h>
#include <time.h>

#ifndef UTIL_H_
#define UTIL_H_

#define ADDR_PTR uint64_t 
#define CYCLES long long int

using namespace std;
CYCLES measure_one_block_access_time(ADDR_PTR addr);

#define cacheSize 32 * 1024
#define ways 8
#define channels 3

class BufferedWriter{
public:
    BufferedWriter(char* text_buf, ADDR_PTR *set): text_buf_(text_buf), cacheSet(set) {} 
    void write();
    void write_one_char(char c);   
    void write_one_bit(bool b);

private:
	char startByte = 0b10101010;
	ADDR_PTR *cacheSet;
    char* text_buf_;
};

class BufferedReader{
public:
    BufferedReader(ADDR_PTR *set): cacheSet(set) {} 
    void read(bool b);
    bool getBit();
    bool doGetBit();

private:
    int counter = 0;
    char buffer = 0;
    bool isReading = false;
    int measureCt = 1000;
	char startByte = 0b10101010;
	ADDR_PTR *cacheSet;

	// if 1s are turning to 0s, decrease.
	// if 0s are turning to 1s, increase.
	// int cutoff = 50000000;
	int cutoff = 50000;
};

CYCLES measure_one_block_access_time(ADDR_PTR addr);
CYCLES rdtsc();
CYCLES getCycleEnd(CYCLES start);
ADDR_PTR* getCacheSet();
bool getBit();
bool doGetBit();

#endif
