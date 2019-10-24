
// You may only use fgets() to pull input from stdin
// You may use any print function to stdout to print 
// out chat messages
#include <stdio.h>

// You may use memory allocators and helper functions 
// (e.g., rand()).  You may not use system().
#include <stdlib.h>

#include <inttypes.h>
#include <time.h>

#ifndef UTIL_H_
#define UTIL_H_

#define ADDR_PTR uint64_t 
#define CYCLES long long unsigned

CYCLES measure_one_block_access_time(ADDR_PTR addr);

void flush(ADDR_PTR addr);

void print_access_time(ADDR_PTR addr);

unsigned long long rdtsc(void);

long long unsigned getCycleEnd(long long unsigned start);

#endif
