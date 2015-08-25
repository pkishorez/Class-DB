/**
 * @file lib_memory.h
 * @author Kishore
 * @date August 6, 2015, 7:01 PM 
 */


// Default values of some important constants used in this library.
#ifndef MEM_BLOCK_SIZE
	#define MEM_BLOCK_SIZE 20
#endif

#ifndef N_MEM_BLOCKS
	#define N_MEM_BLOCKS 10240
#endif

typedef struct{
	char cdata[MEM_BLOCK_SIZE];

	int __next_block;	// -1 if no block is there.
	int __filled;		// Amount of data filled. If equal to MEM_BLOCK_SIZE block filled.


	int __next_free_block;
	int __is_free_block;
}mem_block;


typedef struct{
	int t_length;
	
	int __index;
	int __first_block;
	int __final_block;
	
	int __next_free_block;
	int __is_free_block;
}mem_instance;


struct memory_get{
	int len;
	char *ptr;
	int _index;
};

void memory_init();
int memory_instance_create();
void memory_instance_delete(int index);
int memory_instance_append(int index, char *data, int len);
void memory_instance_print(int index);
void memory_instance_initget(int index, struct memory_get *ch);
void memory_instance_get(int index, struct memory_get *next);