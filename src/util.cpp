
#include "util.hpp"

/* Measure the time it takes to access a block with virtual address addr. */
CYCLES measure_one_block_access_time(ADDR_PTR addr)
{
	CYCLES cycles;

	asm volatile("mov %1, %%r8\n\t"
	"lfence\n\t"
	"rdtsc\n\t"
	"mov %%eax, %%edi\n\t"
	// ".repr 100"
	"mov (%%r8), %%r8\n\t"
	"lfence\n\t"
	// ".endr"
	"rdtsc\n\t"
	"sub %%edi, %%eax\n\t"
	: "=a"(cycles) /*output*/
	: "r"(addr)
	: "r8", "edi");	

	return cycles;
}

void flush(ADDR_PTR addr) {
	asm volatile(
	"clflush (%0)\n\t"
	: 
	: "r"(addr));
}

void print_access_time(ADDR_PTR addr) {
	int time = measure_one_block_access_time(addr);
	printf("access time for %p: %s %i\n", 
		(void *)addr, 
		time < 300 ? "in " : "out",
		time);
}

unsigned long long rdtsc(void)
{
    unsigned hi, lo;
    __asm__ __volatile__ (

	"lfence\n\t"
	"rdtsc\n\t"
	"lfence\n\t"
	: "=a"(lo), "=d"(hi));
    return ( (unsigned long long)lo)|( ((unsigned long long)hi)<<32 );
}