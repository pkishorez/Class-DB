/**
 * @file lib_memory.h
 * @author Kishore
 * @date August 6, 2015, 7:01 PM 
 */

#include "lib_types.h"
void memory_init();

int memory_instance_create();
void memory_instance_delete(int mem_index);

int memory_instance_append(int mem_index, char *data, int len);
int memory_instance_get_len(int mem_index);

int memory_instance_write_t(char *target, memory_get mg, int len);
int memory_instance_copy_t(memory_get mg, char *target, int len);
int memory_instance_copy(int mem_index, int index, char *target, int len);
int memory_instance_copy_string_t(memory_get mg, char *target);
int memory_instance_strlen(memory_get mg);

memory_get memory_instance_get(int mem_index, int index);
memory_get memory_instance_get_end_reference(int mem_index);
int memory_instance_ref_move_ahead(memory_get *mg, int offset);


int memory_instance_get_char(memory_get mg);
int memory_instance_get_and_move_char(memory_get *mg);
int32_t memory_instance_get_int32(memory_get mg);
int64_t memory_instance_get_int64(memory_get mg);

void memory_instance_print(int mem_index);
void memory_instance_print_t(memory_get mg, int len);
void memory_debug();