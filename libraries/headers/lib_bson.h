/**
 * @file lib_bson.h
 * @author Kishore
 * @date October 15, 2015, 8:02 PM 
 */

#include "lib_types.h"

void bson_encode_start(int mem_index);
bson_document bson_encode_end(int mem_index);
void bson_encode_int32(int mem_index, char *key, int32_t value);
void bson_encode_int64(int mem_index, char *key, int64_t value);
void bson_encode_string(int mem_index, char *key, char *value);
void bson_encode_document_start(int mem_index, char *key);
void bson_encode_document_end(int mem_index);

void bson_encode_array_start(int mem_index, char *key);
void bson_encode_array_end(int mem_index);

void bson_encode_binary(int mem_index, char *key, void *value, int32_t len);

bson_property bson_property_get(bson_document doc, char* key);
int32_t bson_get_int32(bson_document doc, char *key);
int64_t bson_get_int64(bson_document doc, char *key);
void bson_get_string(bson_document doc, char *key, char *value, int max_len);
bson_property bson_array_get(bson_array arr, int index);
int bson_array_len(bson_array arr, char *key);

void bson_print(bson_document doc);
//void bson_print_document_details(memory_get mg);
void bson_print_propertystats(bson_property prop);