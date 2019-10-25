
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
#define CYCLES uint32_t

using namespace std;
CYCLES measure_one_block_access_time(ADDR_PTR addr);

class BufferedReader{
public:
    int counter = 0;
    char buffer = 0;  
    char start = 0b10101010;
    bool isReading = false;
    
    void read(bool b);
};


class BufferedWriter{
public:
    char start = 0b10101010;

    BufferedWriter(char* text_buf):text_buf_(text_buf){} 
    void write();

private:
    char* text_buf_;
    void write_one_char(char c);   
    void write_one_bit(bool b);
};

#endif
