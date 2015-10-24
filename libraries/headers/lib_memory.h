/**
 * @file lib_memory.h
 * @author Kishore
 * @date August 6, 2015, 7:01 PM 
 */


// Default values of some important constants used in this library.
#ifndef MEM_BLOCK_SIZE
	#define MEM_BLOCK_SIZE 256
#endif

#ifndef N_MEM_BLOCKS
	#define N_MEM_BLOCKS 1000
#endif

/**
 * @brief structure of memory block used for storage of data.
 * 
 * Memory block is the unit of memory in memory module. Each block
 * caries out information like whether the block is free or already in
 * use etc. Memory block is not visible to the the end user using this module.
 */
typedef struct{
	char cdata[MEM_BLOCK_SIZE];	/**< Data array where data is stored. */

	int __next_block;			/**< Next block this block points to.
								 * -1 if none.*/
	int __filled;				/**< Amount of data filled in this block.*/


	int __next_free_block;		/**< Next free block this block points to, 
								 * if this block is free.*/
	int __is_free_block;		/**< 1 if this is free or 0 if already in use*/
}mem_block;
mem_block k;


typedef struct{
	int t_length;
	
	int __index;
	int __first_block;
	int __final_block;
	
	int __next_free_block;
	int __is_free_block;
}mem_instance;


typedef struct{
	int _instance_index;
	int _block_index;
	int _index;
}memory_get;

void memory_init();

int memory_instance_create();
void memory_instance_delete(int mem_index);

int memory_instance_append(int mem_index, char *data, int len);
int memory_instance_get_len(int mem_index);

int memory_instance_copy_t(memory_get mg, char *target, int len);
int memory_instance_copy(int mem_index, int index, char *target, int len);

memory_get memory_instance_get(int mem_index, int index);
memory_get memory_instance_get_end_reference(int mem_index);


int memory_instance_get_char(memory_get mg);
int memory_instance_get_and_move_char(memory_get *mg);

void memory_instance_print(int mem_index);
void memory_debug();