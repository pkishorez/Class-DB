/**
 * @file lib_bson.c
 * @author Kishore
 * @date July 30, 2015, 10:18 PM
 * 
 * @brief 
 * 
 * 
 */

#include "headers/headers.h"
#include "headers/lib_utilities.h"
#include "headers/lib_memory.h"
#include "headers/lib_bson.h"
#include "headers/lib_json.h"

static int bson_print_document(int size, memory_get mg);
static int move_next_property(memory_get mg, char code);

static int32_t subdoc_sizes[MAX_SUB_DOCUMENTS];
static int32_t subdoc_basesizes[MAX_SUB_DOCUMENTS];
static memory_get subdoc_refs[MAX_SUB_DOCUMENTS];
static int subdoc_no = 0;
static int max_subdoc_no = 0;

static int parsed = 0;

/**
 * Starts encoding a bson document in memory module index `mem_index`.
 * @param mem_index : memory module index in which encoded bson document
 * should be stored.
 */
void bson_encode_start(int mem_index)
{
	assert(subdoc_no==0);
	// assert that mem_index do not have any data.
	//assert(memory_instance_get_len(mem_index)==0);
	subdoc_refs[subdoc_no] = memory_instance_get_end_reference(mem_index);
	subdoc_basesizes[0] = 0;
	subdoc_sizes[0] = 0;
	memory_instance_append(mem_index, "    ", 4);
}


/**
 * Ends encoding bson document in memory module with index `mem_index`.
 * @param mem_index : memory module index where the bson document is stored.
 * @returns bson_document type which is the reference to bson document
 * encoded.
 */
bson_document bson_encode_end(int mem_index)
{
	assert(subdoc_no==0);
	int i=0;
	int32_t t_size = subdoc_sizes[0];
	char *p = (char *)&t_size;
	memory_instance_write_t(p, subdoc_refs[subdoc_no], 4);
	return memory_instance_get(mem_index, 0);
}

/**
 * Encodes 32bit integer type with key `key` into bson document.
 * @param mem_index : memory index where bson document is stored.
 * @param key : Identifier with which the value can be retrieved later.
 * @param value : Value that needs to be stored.
 */
void bson_encode_int32(int mem_index, char *key, int32_t value)
{
	subdoc_sizes[subdoc_no] += memory_instance_append(mem_index, BSON_SINT32,1);
	subdoc_sizes[subdoc_no] += memory_instance_append(mem_index, key, strlen(key)+1);
	subdoc_sizes[subdoc_no] += memory_instance_append(mem_index, (char *)&value, 4);
}

/**
 * Encodes 64bit integer type with key `key` into bson document.
 * @param mem_index : memory index where bson document is stored.
 * @param key : Identifier with which the value can be retrieved later.
 * @param value : Value that needs to be stored.
 */
void bson_encode_int64(int mem_index, char *key, int64_t value)
{
	subdoc_sizes[subdoc_no] += memory_instance_append(mem_index, BSON_SINT64, 1);
	subdoc_sizes[subdoc_no] += memory_instance_append(mem_index, key, strlen(key)+1);
	subdoc_sizes[subdoc_no] += memory_instance_append(mem_index, (char *)&value, 8);
}

/**
 * Encodes string type with key `key` into bson document.
 * @param mem_index : memory index where bson document is stored.
 * @param key : Identifier with which the value can be retrieved later.
 * @param value : Value that needs to be stored.
 */
void bson_encode_string(int mem_index, char *key, char *value)
{
	subdoc_sizes[subdoc_no] += memory_instance_append(mem_index, BSON_SSTRING, 1);
	subdoc_sizes[subdoc_no] += memory_instance_append(mem_index, key, strlen(key)+1);
	subdoc_sizes[subdoc_no] += memory_instance_append(mem_index, value, strlen(value)+1);
}

/**
 * For encoding a nested document in bson.
 * @param mem_index : memory index where bson document is being stored.
 * @param key : Nested document name, so that it could be referenced later.
 */
void bson_encode_document_start(int mem_index, char *key)
{
	subdoc_no++;
	if (max_subdoc_no<subdoc_no)
		max_subdoc_no = subdoc_no;
	assert(subdoc_no<MAX_SUB_DOCUMENTS);
	subdoc_basesizes[subdoc_no] = 0;
	subdoc_sizes[subdoc_no] = 0;

	subdoc_basesizes[subdoc_no] += memory_instance_append(mem_index, BSON_SDOCUMENT,1);
	int slen = strlen(key)+1;
	subdoc_basesizes[subdoc_no] += memory_instance_append(mem_index, key, slen);

	subdoc_refs[subdoc_no] = memory_instance_get_end_reference(mem_index);
	subdoc_basesizes[subdoc_no] += memory_instance_append(mem_index, "    ", 4);
}

/**
 * Ends encoding a nested document.
 * @param mem_index : Memory index where bson document is stored.
 */
void bson_encode_document_end(int mem_index)
{
	assert(subdoc_no>0);

	subdoc_sizes[subdoc_no-1] += subdoc_sizes[subdoc_no];
	subdoc_sizes[subdoc_no-1] += subdoc_basesizes[subdoc_no];
	char * p = (char *)&subdoc_sizes[subdoc_no];
	memory_instance_write_t(p, subdoc_refs[subdoc_no], 4);
	subdoc_no--;
}

/**
 * Same as nested document.
 * @param mem_index : where bson is stored.
 * @param key : Name of array to be stored.
 */
void bson_encode_array_start(int mem_index, char *key)
{
	bson_encode_document_start(mem_index, key);
}

/**
 * Ends array encoding.
 * @param mem_index : memory index where bson is stored.
 */
void bson_encode_array_end(int mem_index)
{
	bson_encode_document_end(mem_index);
}


void bson_encode_binary(int mem_index, char *key, void *value, int32_t len)
{
	subdoc_sizes[subdoc_no] += memory_instance_append(mem_index, BSON_SBINARY,1);
	subdoc_sizes[subdoc_no] += memory_instance_append(mem_index, key, strlen(key)+1);
	subdoc_sizes[subdoc_no] += memory_instance_append(mem_index, (char *)&len, 4);
	subdoc_sizes[subdoc_no] += memory_instance_append(mem_index, value, len);
}


int bson_array_len(bson_array arr, char *key)
{
	if (key!=NULL){
		bson_property prop = bson_property_get(arr, key);
		assert(prop.type==BSON_DOCUMENT);
		arr = prop.pointer;
	}
	int len = 0;
	memory_get mg = arr;
	int32_t size;
	
	int parsed = 0;
	memory_instance_copy_t(mg, (char *)&size, 4);
	parsed += memory_instance_ref_move_ahead(&mg, 4);

	while (parsed <= size){
		char code;
		memory_instance_copy_t(mg, &code, 1);

		parsed += memory_instance_ref_move_ahead(&mg, 1);
		parsed += memory_instance_ref_move_ahead(&mg, memory_instance_strlen(mg)+1);
		parsed += memory_instance_ref_move_ahead(&mg, move_next_property(mg, code));
		
		len++;
	}
	return len;
}
bson_property bson_array_get(bson_array arr, int index)
{
	bson_property p;

	memory_get mg = arr;
	int32_t size;
	
	int parsed = 0;
	memory_instance_copy_t(mg, (char *)&size, 4);
	parsed += memory_instance_ref_move_ahead(&mg, 4);

	int ind = 0;
	while (parsed <= size){
		char code;
		memory_instance_copy_t(mg, &code, 1);
		
		parsed += memory_instance_ref_move_ahead(&mg, 1);
		
		char ki[256];
		int len = memory_instance_copy_string_t(mg, ki)+1;
		parsed += memory_instance_ref_move_ahead(&mg, len);

		if (index==ind++){
			p.pointer = mg;
			p.type = code;
			return p;
		}
		parsed += memory_instance_ref_move_ahead(&mg, move_next_property(mg, code));
	}
	mg._block_index = -1;
	mg._index = -1;
	mg._instance_index = -1;

	p.pointer = mg;
	p.type = BSON_UNKNOWN;
	return p;
}

void bson_get_string(bson_document doc, char *key, char *value, int max_len)
{
	bson_property prop = bson_property_get(doc, key);
	assert(prop.type==BSON_STRING);
	assert(memory_instance_strlen(prop.pointer)<max_len);

	memory_instance_copy_string_t(prop.pointer, value);
}

int32_t bson_get_int32(bson_document doc, char *key)
{
	bson_property prop = bson_property_get(doc, key);
	assert(prop.type==BSON_INT32);

	return memory_instance_get_int32(prop.pointer);
}

int64_t bson_get_int64(bson_document doc, char *key)
{
	bson_property prop = bson_property_get(doc, key);
	assert(prop.type==BSON_INT64);
	
	return memory_instance_get_int64(prop.pointer);
}

bson_property bson_property_get(bson_document doc, char *key)
{
	bson_property p;
	memory_get mg = doc;
	int32_t size;
	
	int parsed = 0;
	memory_instance_copy_t(mg, (char *)&size, 4);
	parsed += memory_instance_ref_move_ahead(&mg, 4);

	while (parsed != size){
		char code;
		memory_instance_copy_t(mg, &code, 1);
		
		parsed += memory_instance_ref_move_ahead(&mg, 1);
		
		char ki[256];
		int len = memory_instance_copy_string_t(mg, ki)+1;
		parsed += memory_instance_ref_move_ahead(&mg, len);

		if (!strcmp(key, ki)){
			p.pointer = mg;
			p.type = code;
			return p;
		}
		parsed += memory_instance_ref_move_ahead(&mg, move_next_property(mg, code));
	}
	mg._block_index = -1;
	mg._index = -1;
	mg._instance_index = -1;
	
	p.pointer = mg;
	p.type = BSON_UNKNOWN;
	return p;
}

void bson_print_propertystats(bson_property prop)
{
	switch(prop.type){
		case BSON_BINARY : {
			printf("Binary\n");
			break;
		}
		case BSON_DOCUMENT:{
			printf("Document\n");
			break;
		}
		case BSON_INT32:{
			printf("INT32 : %"PRId32"\n", memory_instance_get_int32(prop.pointer));
			break;
		}
		case BSON_INT64:{
			printf("INT64 : %"PRId64"\n", memory_instance_get_int64(prop.pointer));
			break;
		}
		case BSON_STRING:{
			printf("STRING\n");
			break;
		}
		default:{
			printf("UNKNOWN\n");
			break;
		}
	}
}
void bson_print(bson_document doc)
{
	if (doc._instance_index==-1){
		printf("INVALID DOCUMENT...\n");
		return;
	}
	int sub_doc = 0;

	int parsed = 0;
	memory_get mg = doc;
	int32_t size;
	memory_instance_copy_t(mg, (char *)&size, 4);
	memory_instance_ref_move_ahead(&mg, 4);

	printf("\n");
	json_encode_start();
	bson_print_document(size, mg);
	json_encode_end();
}


static int bson_print_document(int size, memory_get mg)
{
	int parsed = 0;
	while (parsed != size)
	{
		char code;
		memory_instance_copy_t(mg, &code, 1);
		parsed += memory_instance_ref_move_ahead(&mg, 1);

		char key[250];
		int mv = memory_instance_copy_string_t(mg, key);
		parsed += memory_instance_ref_move_ahead(&mg, mv+1);
		switch (code)
		{
			case BSON_INT32:{
				int32_t num;
				memory_instance_copy_t(mg, (char *)&num, 4);
				json_add_number(key, num);
				memory_instance_ref_move_ahead(&mg, 4);
				parsed += 4;
				fflush(stdout);
				break;
			}
			case BSON_INT64:{
				int64_t num;
				memory_instance_copy_t(mg, (char *)&num, 8);
				json_add_number(key, num);
				memory_instance_ref_move_ahead(&mg, 8);
				parsed += 8;
				break;
			}
			case BSON_STRING:{
				char value[500];
				int mv = memory_instance_copy_string_t(mg, value);
				parsed += memory_instance_ref_move_ahead(&mg, mv+1);
				json_add_string(key, value);
				break;
			}
			case BSON_DOCUMENT:{
				int32_t sz;
				memory_instance_copy_t(mg, (char *)&sz, 4);
				parsed += memory_instance_ref_move_ahead(&mg, 4);
				json_nested(key);
				int ps = bson_print_document(sz, mg);
				parsed += memory_instance_ref_move_ahead(&mg, ps);
				json_unested();
				break;
			}
			default:{
				return -1;
			}
		}
	}
	return parsed;
}


static int move_next_property(memory_get mg, char code)
{
	int len = 0;
	switch(code){
		case BSON_INT32:{
			return 4;
		}
		case BSON_INT64:{
			return 8;
		}
		case BSON_STRING:{
			return memory_instance_strlen(mg)+1;
		}
		case BSON_BINARY:
		case BSON_DOCUMENT:{
			int32_t len = memory_instance_get_int32(mg);
			return len+4;
		}
		default:{
			printf("PROPERTY NOT FOUND\n");
			return 0;
		}
	}
}