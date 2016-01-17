/**
 * @file dm_fixed_collection.h
 * @author Kishore
 * @date December 27, 2015, 10:39 AM 
 */


#include "dm_types.h"
void fixed_collection_init(bson_document document, collection_fixed_t *config);
void fixed_collection_config_save(int mem_index, collection_fixed_t *config);