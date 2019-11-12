### Compiling and running
Run `make` and `make run` in `./src/receiver/` and `./src/sender`.

### How our implementation works
1. Allocate an array as big as the L1 data cache (32kb). 
2. Iterate through the array and find addresses which are assigned to the same set (i.e. the last 12 bits are identical).
3. Repeat step 2 to find 16 sets which will be used for sending data.
4. Sender sends data by writing each address in the 16 cache sets.
5. Receiver recieves data by reading each of the addresses, and comparing the access times against a threshold value.

##### Sending a bit
The sender and receiver synchronize by starting a new period every 2^25 cycles. To send a 1, the sender writes to all previously chosen sets for the duration of the period. To send a 0, the sender busy waits. The receiver reads the cache lines 10,000 times and compares the elapsed time to a threshold value to distinguish between 1 and 0. Then, it waits until the end of the period.

##### Sending a character
Each character is prefixed by a byte of all 1's, which is easy to detect and ensures no characters are lost and no unsent characters appear. After this byte, the next 8 bits are interpreted as an ASCII character and printed to the screen.

### Challenges we ran into
At first we struggled to understand the capabilities of our CPUs. The first didn't support taskset (Mac). The second didn't support hyperthreading. The third we assumed had four cores running one thread each, instead of two cores with two threads each. We completed the project quickly after understanding our hardware, but unfortunately ran out of time before optimizing our solution.

### Extra credit
None.
