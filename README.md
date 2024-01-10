# vrtmem
This is a C program to mock virtual memory system and get an experiment with it.\
After choosing the page replacement algorithm, the system will accept commands to read, write from (or to a) virtual address space from the user. For our experimental purpose, we define the specification as follows:
- The Vm system has 128 addresses
- The main memory has 32 addresses
- Each page has 8 addresses containing page number, valid bit, and a dirty bit
- Each address in memory stores a single integer

To run the program, the user would have to first decide which page replacement algorithm to use. There are two options which the program provides: FIFO and LRU.
1. As its name suggest, FIFO is the simplest age-replacement algorithm which needs a low-overhead with little bookkeeping part. While it's a cheap option to use, it performs poorly in practical application.
2. The least recently used (LRU) page replacement algorithm works on the idea that pages that have been most heavily used in the past few instructions are most likely to be used heavily in the next few instructions too. In theory, this is the most near-optimal solution but rather expensive to implement in practice. For more information about page replacement algorithms, follow this [link](https://en.wikipedia.org/wiki/Page_replacement_algorithm) (wiki) to learn.

Assuming that the user have chosen the algorithm, user has to provide the name of algorithm as an `argv` to the program.
> If the user doesn't specify the algorithm, the default mode is FIFO.

```
gcc -o vm vm.c
./vm LRU
```
