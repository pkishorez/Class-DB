/**
 * @file lib_memory.c
 * @author Kishore
 * @date August 6, 2015, 6:59 PM
 * 
 * @brief 
 * 
 * 
 */

#include "headers/headers.h"
#include "headers/lib_memory.h"


static mem_block mem_blocks[N_MEM_BLOCKS];
static mem_instance instances[N_MEM_BLOCKS];

static int first_free_memblock = 0;
static int first_free_instance = 0;


static int get_free_block();
static void make_free_block(int n);
static int append_memory_block(int index, char *data, int len);
void static _instance_get(int index, struct memory_get *ch, int is_new);


void memory_init()
{
	int i;
	for (i=0; i<N_MEM_BLOCKS; i++)
	{
		instances[i].__next_free_block = i+1;
		instances[i].__is_free_block = 1;

		mem_blocks[i].__next_free_block = i+1;
		mem_blocks[i].__is_free_block = 1;
	}
	instances[i-1].__next_free_block = -1;
	mem_blocks[i-1].__next_free_block = -1;
}


/**
 * Get a free block of memory from _blocks array.
 * @returns index of first free block and -1 on error.
 */
static int get_free_block()
{
	if (first_free_memblock==-1)
		return -1;
	
	int f_free_block = first_free_memblock;
	first_free_memblock = mem_blocks[first_free_memblock].__next_free_block;
	
	mem_blocks[f_free_block].__is_free_block = 0;
	mem_blocks[f_free_block].__filled = 0;
	mem_blocks[f_free_block].__next_block = -1;
	return f_free_block;
}


/**
 * Makes the block at index n as free block.
 * @param n : index of block that should be freed. Gives error if the block
 * n is already free.
 */
static void make_free_block(int n)
{
	assert(n<N_MEM_BLOCKS);
	assert(!mem_blocks[n].__is_free_block);
	mem_blocks[n].__is_free_block = 1;
	
	mem_blocks[n].__next_free_block = first_free_memblock;

	first_free_memblock = n;
}

static int append_memory_block(int index, char *data, int len)
{
	assert(index<N_MEM_BLOCKS);
	assert(!mem_blocks[index].__is_free_block);
	
	int capacity = MEM_BLOCK_SIZE - mem_blocks[index].__filled;
	if (capacity>=len){
		strncpy(mem_blocks[index].cdata + mem_blocks[index].__filled, data, len);
		mem_blocks[index].__filled += len;
		return len;
	}
	else{
		strncpy(mem_blocks[index].cdata + mem_blocks[index].__filled, data, capacity);
		mem_blocks[index].__filled += capacity;
		return capacity;
	}
}
int memory_instance_create()
{
	if (first_free_instance==-1)
		return -1;

	
	int f_free_instance = first_free_instance;
	first_free_instance = instances[first_free_instance].__next_free_block;
	
	instances[f_free_instance].__is_free_block = 0;
	
	// Create a new memory block and set default values to memory instance.
	int mem_block = get_free_block();
	instances[f_free_instance].__first_block = mem_block;
	instances[f_free_instance].__final_block = mem_block;
	instances[f_free_instance].__index = 0;
	instances[f_free_instance].t_length = 0;

	return f_free_instance;
}

void memory_instance_delete(int n)
{
	assert(n<N_MEM_BLOCKS);
	assert(!instances[n].__is_free_block);
	instances[n].__is_free_block = 1;
	
	int f_block = first_free_instance;
	instances[n].__next_free_block = f_block;
	
	/* Clean up of memory blocks associated with memory instance of index.*/
	int i = instances[n].__first_block;
	
	while (i!=-1){
		int j = mem_blocks[i].__next_block;
		make_free_block(i);
		i = j;
	}
	
	first_free_instance = n;
}


/**
 * Append data to the memory instance of index `index`
 * @param index : index of memory instance to which data should be appended.
 * @param data : Target data that should be appended.
 * @param len : Length of data provided for appending.
 * @returns length of data that is copied.
 * 
 * This function appends data of length `len` to the memory instace
 * with index `index`. It returns the amount of data appended successfully.
 * Return value less than the length provided means, insufficient storage
 * occured.
 */
int memory_instance_append(int index, char *data, int len)
{
	assert(!instances[index].__is_free_block);

	int copied = 0;
	int l_block = instances[index].__final_block;
	while (copied != len){
		int cp = append_memory_block(l_block, data+copied, len-copied);
		if (cp==0){
			int block = l_block;
			l_block = get_free_block();
			if (l_block==-1){
				return copied;
			}
			mem_blocks[block].__next_block = l_block;
			instances[index].__final_block = l_block;
		}
		else{
			copied += cp;
			instances[index].t_length += cp;
		}
	}
	return len;
}


void memory_instance_print(int index)
{
	assert(index<N_MEM_BLOCKS);
	assert(!instances[index].__is_free_block);
	int f_block = -1;
	int l_block = instances[index].__final_block;
	
	//printf("TOTAL LENGTH OF STRING IS : %d\n", instances[index].t_length);
	do{
		if (f_block==-1)
			f_block = instances[index].__first_block;
		else
			f_block = mem_blocks[f_block].__next_block;
		int i=0;
		char *buffer = mem_blocks[f_block].cdata;
		int size = mem_blocks[f_block].__filled;
		//printf("\n\n=======NEW BLOCK==========%d\n", size);
		for (i=0; i<size; i++)
			printf("%c", buffer[i]);
	}
	while ((f_block!=l_block));
	printf("<END>");
}


int static _instance_get(int index, struct memory_get *ch, int is_new)
{
	assert(index<N_MEM_BLOCKS);
	assert(!instances[index].__is_free_block);

	int block;
	if (is_new){
		block = instances[index].__first_block;
		if (block==-1)
			return 0;
	}
	else{
		assert(ch->_index!=-1);
		block = mem_blocks[ch->_index].__next_block;
		if (block==-1){
			ch->_index = -1;
			ch->len = 0;
			ch->ptr = NULL;
			return 0;
		}
	}
	ch->_index = block;
	ch->len = mem_blocks[block].__filled;
	ch->ptr = mem_blocks[block].cdata;
	return 1;
}


/**
 * Initiate get request to retrieve data in a controlled manner.
 * @param index : Memory instance index.
 * @param ch : struct memory_get object where the details of
 * get request are updated.
 */
int memory_instance_initget(int index, struct memory_get *ch)
{
	return _instance_get(index, ch, 1);
}


/**
 * 
 * @param index
 * @param ch
 */
int memory_instance_get(int index, struct memory_get *ch)
{
	return _instance_get(index, ch, 0);
}