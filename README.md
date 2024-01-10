# vrtmem
This is a C program to mock virtual memory system and get an experiment with it.\
After choosing the page replacement algorithm, the system will accept commands to read, write from (or to a) virtual address space from the user. For our experimental purpose, we define the specification as follows:
1. The Vm system has 128 addresses
2. The main memory has 32 addresses
3. Each page has 8 addresses containing page number, valid bit, and a dirty bit
4. Each address in memory stores a single integer
