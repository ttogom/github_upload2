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

There are 4 basic functions inside the program to manage the virtual memory system.
1. `read <virtual_addr>` : Prints the contents inside the memory address. When a page fault occurs, it prints "A Page Fault Has Occurred" before the contents of a memory address.
2. `write <virtual_addr> <num>` : Writes data (num) to a specific memory location. When a page fault occurs, it prints "A Page Fault Has Occurred" is printed on the screen.
3. `showmain <ppn>` : Prints the contents of a physical page in the main memory. Since each page contains eight addresses, eight contents of addresses should be printed and together with their associated physical addresses.
4. `showptable` : Prints the contents of the page table. The system has 16 virtual pages. Each page table entry contains three fields which is Valid bit, Dirty bit, and a Page number.
- contents are printed by the order of virtual_page_num:valid_bit:dirty_bit:page_num
  0:1:0:0
  1:1:1:3
  2:0:0:2
