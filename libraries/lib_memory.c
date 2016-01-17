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



/**
 * Get a free block of memory from _blocks array.
 * @returns index of the free block or -1 if no blocks are available
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
	assert(n<N_MEM_BLOCKS && n>=0);
	assert(!mem_blocks[n].__is_free_block);
	mem_blocks[n].__is_free_block = 1;
	mem_blocks[n].__next_free_block = first_free_memblock;
	first_free_memblock = n;
}



/**
 * Init memory module with all required values. This function should
 * be called if the project uses memory module. Any functions in this
 * module should be followed by this function call.
 */
void memory_init()
{
	assert(N_MEM_BLOCKS>=1 && MEM_BLOCK_SIZE>=1);
	int i;
	for (i=0; i<N_MEM_BLOCKS; i++)
	{
		instances[i].__next_free_block = i+1;
		instances[i].__is_free_block = 1;
		instances[i].t_length = 0;
		instances[i].__final_block = -1;
		instances[i].__first_block = -1;

		mem_blocks[i].__next_free_block = i+1;
		mem_blocks[i].__is_free_block = 1;
		mem_blocks[i].__next_block = -1;
	}
	instances[i-1].__next_free_block = -1;
	mem_blocks[i-1].__next_free_block = -1;

	first_free_memblock = 0;
	first_free_instance = 0;
}

/**
 * Creates memory instance and returns the reference of it.
 * @returns integer, which is reference to the newly created memory instance.
 * 
 * Creates memory instance and returns reference to the newly created
 * one.
 */
int memory_instance_create()
{
	if (first_free_instance==-1)
		return -1;


	int new_instance = first_free_instance;
	first_free_instance = instances[first_free_instance].__next_free_block;

	instances[new_instance].__is_free_block = 0;

	// Create a new memory block and set default values to memory instance.
	int f_block = get_free_block();
	if (f_block==-1){
		memory_instance_delete(new_instance);
		return -1;
	}
	mem_blocks[f_block].__next_block = -1;
	instances[new_instance].__first_block = f_block;
	instances[new_instance].__final_block = f_block;
	instances[new_instance].__index = 0;
	instances[new_instance].t_length = 0;

	return new_instance;
}

/**
 * Deletes memory instance with index n.
 * @param n : memory instance index.
 * 
 * This function deletes the memory instance of index n,
 * along with all the memory blocks associated with it.
 */
void memory_instance_delete(int mem_index)
{
	assert((mem_index<N_MEM_BLOCKS) && (mem_index>=0));
	assert(!instances[mem_index].__is_free_block);
	
	/* Clean up of memory blocks associated with memory instance of index.*/
	int i = instances[mem_index].__first_block;
	
	while (i!=-1){
		int j = mem_blocks[i].__next_block;
		make_free_block(i);
		i = j;
	}

	instances[mem_index].__is_free_block = 1;
	
	int f_block = first_free_instance;
	instances[mem_index].__next_free_block = f_block;
	first_free_instance = mem_index;

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
int memory_instance_append(int memins_index, char *data, int len)
{
	assert(!instances[memins_index].__is_free_block);
	assert(len>=0);

	int copied = 0;

	int l_block = instances[memins_index].__final_block;

	while (copied != len){
		int capacity = MEM_BLOCK_SIZE - mem_blocks[l_block].__filled;
		if (capacity >= (len-copied)){
			memcpy(mem_blocks[l_block].cdata + mem_blocks[l_block].__filled, data+copied, len-copied);
			mem_blocks[l_block].__filled += len-copied;
			instances[memins_index].t_length += len-copied;
			instances[memins_index].__final_block = l_block;
			return len;
		}
		else{
			memcpy(mem_blocks[l_block].cdata + mem_blocks[l_block].__filled, data+copied, capacity);
			copied += capacity;
			mem_blocks[l_block].__filled += capacity;
			instances[memins_index].t_length += capacity;

			int new_block = get_free_block();
			if (new_block==-1){
				return copied;
			}
			mem_blocks[l_block].__next_block = new_block;
			instances[memins_index].__final_block = new_block;
			l_block = new_block;
		}
	}
}


/**
 * Gets the length of data stored in memory instance `mem_index`.
 * @param mem_index : memory instance whose length is to be returned
 * @returns length of data stored in memory instance `mem_index`.
 */
int memory_instance_get_len(int mem_index)
{
	assert((mem_index>=0) && (mem_index<N_MEM_BLOCKS) && (!instances[mem_index].__is_free_block));
	return instances[mem_index].t_length;
}

/**
 * Copies data from memory instance `mem_index` of length `len` starting
 * from `index` to `target`.
 * @param mem_index : memory instance whose data should be copied.
 * @param index : offset from where copying should start in memory instance.
 * @param target : string where data should be copied.
 * @param len : Length of data that should be copied from index.
 * @returns length of data copied.
 */
int memory_instance_copy(int mem_index, int index, char *target, int len)
{
	memory_get mg = memory_instance_get(mem_index, index);
	return memory_instance_copy_t(mg, target, len);
}

/**
 * Copies data of length `len` from reference point to target.
 * @param mg : reference point from where data should be start copying.
 * @param target : target string where data should be copied.
 * @param len : length of data that should be copied starting from `mg`
 * @returns length of data copied.
 */
int memory_instance_copy_t(memory_get mg, char *target, int len)
{
	int copied = 0;

	int block = mg._block_index;
	int index = mg._index;
	while (copied != len)
	{
		int capacity = mem_blocks[block].__filled - index;
		if (capacity >= (len-copied)){
			memcpy(target+copied, mem_blocks[block].cdata + index, len-copied);
			return len;
		}
		else{
			memcpy(target+copied, mem_blocks[block].cdata + index, capacity);
			copied += capacity;
			block = mem_blocks[block].__next_block;
			if (block == -1)
				return copied;
			index = 0;
		}
	}
	return copied;
}


int memory_instance_write_t(char *target, memory_get mg, int len)
{
	int copied = 0;

	int block = mg._block_index;
	int index = mg._index;
	while (copied != len)
	{
		int capacity = mem_blocks[block].__filled - index;
		if (capacity >= (len-copied)){
			memcpy(mem_blocks[block].cdata + index, target+copied , len-copied);
			return len;
		}
		else{
			block = mem_blocks[block].__next_block;
			if (block==-1)
				return copied;
			index = 0;
		}
	}
	return copied;
}

int memory_instance_copy_string_t(memory_get mg, char *target)
{
	int i=0;
	int ch;
	while ((ch=memory_instance_get_and_move_char(&mg)) != '\0'){
		target[i++] = (char)ch;
	}
	target[i] = '\0';
	return i;
}

int memory_instance_strlen(memory_get mg)
{
	int len=0;
	while (memory_instance_get_and_move_char(&mg) != '\0'){
		len++;
	}
	return len;
}

/**
 * Get a reference to memory instance at position `index`.
 * @param mem_index : memory instance index
 * @param index : position at which reference is needed.
 * @returns memory instance reference at position `index`
 * 
 * Reference is used to carry out operations like get character,
 * copy memory from reference point till specified length etc. Reference
 * is like index in array which points a particular element in it.
 */
memory_get memory_instance_get(int mem_index, int index)
{
	static memory_get mg;
	mg._instance_index = mem_index;

	if (instances[mem_index].__is_free_block){
		mg._block_index = -1;
		mg._index = -1;
		return mg;
	}
	int block = instances[mem_index].__first_block;
	int filled = mem_blocks[block].__filled;
	while (index>=filled){
		block = mem_blocks[block].__next_block;
		if (block==-1){
			mg._block_index = -1;
			mg._index = -1;
			return mg;
		}
		filled += mem_blocks[block].__filled;
	}
	mg._block_index = block;
	mg._index = mem_blocks[block].__filled - (filled - index);
	return mg;
}


/**
 * Gets reference of memory instance at end positon.
 * @param mem_index : memory instance whose end position reference is needed.
 * @returns reference of memory instance's end point.
 * @see `memory_instance_get`
 */
memory_get memory_instance_get_end_reference(int mem_index)
{
	assert((mem_index>=0) && (mem_index<N_MEM_BLOCKS) && (!instances[mem_index].__is_free_block));
	memory_get mg;
	mg._instance_index = mem_index;

	int l_block = instances[mem_index].__final_block;
	mg._block_index = l_block;
	mg._index = mem_blocks[l_block].__filled;
	return mg;
}

/**
 * Move the refence point `offset` steps ahead.
 * @param mg : reference point that needs to be moved.
 * @param offset : offset that the reference should be moved.
 * @returns number of steps moved.
 */
int memory_instance_ref_move_ahead(memory_get *mg, int offset)
{
	int moved = 0;
	while (moved!=offset){
		if (mg->_block_index==-1)
			return moved;
		if (mem_blocks[mg->_block_index].__filled > (mg->_index+1)){
			mg->_index++;
		}else{
			mg->_block_index = mem_blocks[mg->_block_index].__next_block;
			mg->_index = 0;
		}
		moved++;
	}
	return moved;
}

/**
 * Gets character at reference `mg`
 * @param mg : reference point of an instance.
 * @returns character at reference point `mg`
 */
int memory_instance_get_char(memory_get mg)
{
	if (mg._block_index==-1)
		return -1;
	return mem_blocks[mg._block_index].cdata[mg._index];
}


int32_t memory_instance_get_int32(memory_get mg)
{
	int32_t target;
	memory_instance_copy_t(mg, (char *)&target, 4);
	return target;
}


int64_t memory_instance_get_int64(memory_get mg)
{
	int64_t target;
	memory_instance_copy_t(mg, (char *)&target, 8);
	return target;
}

/**
 * Gets character at reference `mg` and moves the reference
 * step ahead.
 * @param mg : pointer to reference.
 * @returns character at reference `mg`
 */
int memory_instance_get_and_move_char(memory_get *mg)
{
	if (mg->_block_index==-1)
		return -1;
	int ch = mem_blocks[mg->_block_index].cdata[mg->_index];
	if (mem_blocks[mg->_block_index].__filled > (mg->_index+1)){
		mg->_index++;
		return ch;
	}else{
		mg->_block_index = mem_blocks[mg->_block_index].__next_block;
		mg->_index = 0;
		return ch;
	}
}


/**
 * Prints the data in memory instance referred by mem_index.
 * @param mem_index : whose data needs to be printed.
 */
void memory_instance_print(int mem_index)
{
	assert((mem_index>=0) && (mem_index<=N_MEM_BLOCKS) && (!instances[mem_index].__is_free_block));
	int block = instances[mem_index].__first_block;
	while (block!=-1){
		int i=0;
		for (i=0; i<mem_blocks[block].__filled; i++){
			if (isprint(mem_blocks[block].cdata[i])){
				printf("%c", mem_blocks[block].cdata[i]);
			}
			else
				printf("%c", mem_blocks[block].cdata[i] & 0xff);
		}
		block = mem_blocks[block].__next_block;
	}
}


void memory_instance_print_t(memory_get mg, int len)
{
	int i=0;
	for (i=0; i<len; i++)
		printf("%c", (char)memory_instance_get_and_move_char(&mg));
}