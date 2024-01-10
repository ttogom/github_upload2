#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MIN(x, y) (((x) < (y)) ? (x) : (y))

struct Memory 
{
    int address;
    int data;
};

struct PageTable 
{
    int v_page_num;                 // virtual memory page_num
    int valid_bit;
    int dirty_bit;
    int page_num;                   // main memory page_num
    int time_stamp;
};

struct Memory main_memory[32];      // key: addr[0, 31], value: data
struct Memory virtual_memory[128];  // key: addr[0, 127], value: data
struct PageTable page_table[16];    // each 8 address

int FIFO = 0;
int LRU = 0;

int global_fifo = 1;
int global_lru = 1;

int physical_page_counter = 0;

bool is_not_valid_bit_count()
{
    int valid_bit_count = 0;
    for (int i = 0; i < 16; i++)
    {
        if (page_table[i].valid_bit == 1) 
        {
            valid_bit_count++;
        }
    }

    if (valid_bit_count > 3)
    {
        return false;
    }

    return true;
}

int kickout()
{
    int minimum = -1;
    for (int i = 0; i < 16; i++)
    {
        if (page_table[i].valid_bit == 1) 
        {
            minimum = page_table[i].time_stamp;
            break;
        }
    }

    for (int i = 0; i < 16; i++)
    {
        if (page_table[i].valid_bit == 1) 
        {
            minimum = MIN(minimum, page_table[i].time_stamp);
        }
    }

    int kicked_out_page;

    for (int i = 0; i < 16; i++)
    {
        if (page_table[i].time_stamp == minimum) 
        {
            page_table[i].valid_bit = 0;

            page_table[i].dirty_bit = 0;
            kicked_out_page = page_table[i].page_num;

            int start = page_table[i].page_num * 8;
            int end = (page_table[i].page_num + 1) * 8;
        
            for(int i = start; i < end; i++)
            {
                main_memory[i].data = -1;
            }

            page_table[i].page_num = i;
        }
    }

    return kicked_out_page;
}

void read(int virtual_address)
{
    // read function
    int virtual_page_number = virtual_address / 8;  // (0 ~ 7) / 8 = 0,   (8 ~ 15) / 8 = 1 ... etc   -> index or page number of v_page_num
    int offset = virtual_address % 8;               // 0 % 8 = 0, 1 % 8 = 1, 2 % 8 = 2 ... etc       -> offset that will be added

    if(page_table[virtual_page_number].valid_bit == 0)
    {
        printf("A Page Fault Has Occurred\n");                                                  // print page fault occur message before content of memory address
        int main_address = page_table[virtual_page_number].page_num;
        main_address = main_address * 8;
        main_address = main_address + offset;

        printf("%d\n", main_memory[main_address].data);    // print content of mapped memory address
        
        if ((1 == FIFO || 1 == LRU) && (is_not_valid_bit_count() == false) ) 
        { 
            int eviected_main_page = kickout();
            page_table[virtual_page_number].page_num = eviected_main_page;
            page_table[virtual_page_number].valid_bit = 1;
        }
        else
        {
            page_table[virtual_page_number].page_num = physical_page_counter ++;
            page_table[virtual_page_number].valid_bit = 1;
        }

        if (1 == LRU)
        {
            page_table[virtual_page_number].time_stamp = global_lru++;
        }
        if (1 == FIFO)
        {
            page_table[virtual_page_number].time_stamp = global_fifo++;
        }
    }
    else
    {   
        if (1 == LRU)
        {
            page_table[virtual_page_number].time_stamp = global_lru++;
        }

        int main_address = page_table[virtual_page_number].page_num;
        main_address = main_address * 8;
        main_address = main_address + offset;
        printf("%d\n", main_memory[main_address].data);    // print content of mapped memory address (-1 if it is stored data, shouldn't be -1 otherwise)
    }
}

void write(int virtual_address, int number_data)
{
    int virtual_page_number = virtual_address / 8;  // (0 ~ 7) / 8 = 0,   (8 ~ 15) / 8 = 1 ... etc   -> index or page number of v_page_num
    int offset = virtual_address % 8;               // 0 % 8 = 0, 1 % 8 = 1, 2 % 8 = 2 ... etc       -> offset that will be added

    if(page_table[virtual_page_number].valid_bit == 0)
    {
        printf("A Page Fault Has Occurred\n"); // print page fault occur message before content of memory address

        if ((1 == FIFO || 1 == LRU) && (is_not_valid_bit_count() == false)) 
        {
            int eviected_main_page = kickout(offset);
            page_table[virtual_page_number].page_num = eviected_main_page;
            page_table[virtual_page_number].valid_bit = 1;
            page_table[virtual_page_number].dirty_bit = 1;

            int main_address = page_table[virtual_page_number].page_num;
            main_address = main_address * 8;
            main_address = main_address + offset;
            main_memory[main_address].data = number_data;
        }
        else
        {
            page_table[virtual_page_number].page_num = physical_page_counter ++;
            page_table[virtual_page_number].valid_bit = 1;
            page_table[virtual_page_number].dirty_bit = 1;

            int main_address = page_table[virtual_page_number].page_num;
            main_address = main_address * 8;
            main_address = main_address + offset;
            main_memory[main_address].data = number_data;
        }

        if (1 == LRU)
        {
            page_table[virtual_page_number].time_stamp = global_lru++;
        }

        if (1 == FIFO)
        {
            page_table[virtual_page_number].time_stamp = global_fifo++;
        }
    }
    else 
    {
        int main_address = page_table[virtual_page_number].page_num;
        main_address = main_address * 8;
        main_address = main_address + offset;

        main_memory[main_address].data = number_data;
        page_table[virtual_page_number].dirty_bit = 1;

        if (1 == LRU)
        {
            page_table[virtual_page_number].time_stamp = global_lru++;
        }
    }
}

void showmain(int ppn)
{
    // showmain function
    // starting = ppn * 8, end = (ppn + 1) * 8 - 1
    // if ppn = 1, main memory address will be (8 ~ 15)

    for(int i = ppn * 8; i < (ppn + 1) * 8; i++)
    {
        printf("%d: %d\n", i, main_memory[i].data);
    }
}

void showptable()
{
    //showtable function
    for(int i = 0; i < 16; i++)
    {
        printf("%d:%d:%d:%d\n", page_table[i].v_page_num, page_table[i].valid_bit, page_table[i].dirty_bit, page_table[i].page_num);
    }
}

void loop() 
{
    char input[100];                // whole input that will be tokenized
    char input_copy[100];           // copy whole input to save original input
    char *words[100] = {NULL};      // args[0] = word, args[1] = word ...
    char *command;                  // = args[0] or First word of input
    char *arg;

    while (1) 
    {
        for(int i = 0; i < 100; i++) // initialize input each iteration to avoid null problem
        {
            input[i] = '\0';
        }

        printf("> "); // print out "> "

        
        fgets(input, sizeof(input), stdin);
        strcpy(input_copy, input); // copy input to input_copy
        arg = strtok(input, " \n\t");

        int i = 0;
        int max_index = 0;

        while (arg != NULL) // tokenize input to words,
        {
            words[i] = arg;
            max_index = i;
            i++;
            arg = strtok(NULL, " \n\t");
        }

        command = words[0]; // command = first word of input

        if (command == NULL)  // if it is just enter or nothing, then retry
        {
            continue;
        }

        if (strcmp(command, "read") == 0) // read command
        {
            if (words[1] == NULL) 
            {
                printf("Error: No virtual address provided.\n");
            } 
            else 
            {
                int address = atoi(words[1]);
                read(address);
            }
        }
        else if (strcmp(command, "write") == 0) // wrtie command
        {
            if (words[1] == NULL) 
            {
                printf("Error: Virtual address or number are not provided.\n");
            } 
            else 
            {
                int address = atoi(words[1]);
                int number = atoi(words[2]);
                write(address, number);
            }
        } 
        else if (strcmp(command, "showmain") == 0) // showmain command
        {
            if (words[1] == NULL) 
            {
                printf("Error: No ppn provided.\n");
            } 
            else 
            {
                int ppn = atoi(words[1]);
                showmain(ppn);
            }
        } 
        else if (strcmp(command, "showptable") == 0) // showtable command
        {
            showptable();
        }
        else if (strcmp(command, "quit") == 0) // quit command
        {
            break;
        } 
    }
}


int main(int argc, char** argv) {

    if (argv[1] == NULL || strcmp (argv[1], "FIFO") == 0)
    {
        FIFO = 1; // if typed FIFO, then set FIFO as 1
    }
    else if (strcmp (argv[1], "LRU") == 0)
    {
        LRU = 1; // if typed LRU, then set LRU as 1
    }

    for(int i = 0; i < 16; i ++) // set valid_bid of page table as 0 before start
    {
        page_table[i].dirty_bit = 0;
        page_table[i].valid_bit = 0;
        page_table[i].page_num = i;
        page_table[i].v_page_num = i;
    }

    for(int i = 0; i < 32; i ++) // set main memory location as -1 before start
    {
        main_memory[i].data = -1;
        main_memory[i].address = i;
    }

    for(int i = 0; i < 128; i ++) // set virtual memory location as -1 before strat
    {
        virtual_memory[i].data = -1;
        virtual_memory[i].address = i;
    }
    loop(); // after set FIFO and LRU, do infinite loop until typed "quit"
    return 0;
}