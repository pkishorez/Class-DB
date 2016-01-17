/**
 * @file dm_collection.c
 * @author Kishore
 * @date December 7, 2015, 5:18 PM
 * 
 * @brief 
 * 
 * {
 *		"collections" : [
 *			{
 *				"name" : {collection_name},
 *				"type" : {collection_type},
 *				"config" : {
 *					// configuration corresponding to related collection type.
 *				}
 *			},
 *			...
 *			...
 *			...
 *		]
 * }
 */

#include "../../libraries/headers/headers.h"
#include "../headers/dm_collection.h"
#include "../headers/dm_fixed_collection.h"
#include "../../libraries/headers/lib_fileio.h"
#include "../../libraries/headers/lib_memory.h"
#include "../../libraries/headers/lib_bson.h"
#include "../../libraries/headers/lib_fileio.h"

collection_t *dm_collections;

char dm_config_path[256]="data/config.cfg";
const char dm_data_path[256]="data/";

/**
 * 
 * @param config
 */
void collection_init(char *config)
{
	if (config==NULL)
		config = dm_config_path;
	
	int mem_index = fileio_map_filetomemory(config);
	
	bson_document bson = memory_instance_get(mem_index, 0);
	
	/****************************
	 * Initializing COLLECTIONS...
     *****************************/
	bson_property collections = bson_property_get(bson, "collections");
	assert(collections.type == BSON_DOCUMENT);
	
	int t_collections = bson_array_len(collections.pointer, NULL);
	
	collection_t *dm_coll;
	int i;
	for (i=0; i<t_collections; i++)
	{
		collection_t *mall = (collection_t *)malloc(sizeof(collection_t));
		if (i!=0)
			dm_coll->_next = mall;
		else
			dm_collections = mall;
		dm_coll = mall;

		bson_property collection = bson_array_get(collections.pointer, i);
		
		// Assign type to dm_collections
		dm_coll->type = bson_get_int32(collection.pointer, "type");

		// Assign name to dm_collections
		bson_get_string(collection.pointer, "name", dm_coll->name, COLLECTION_NAME_LEN);

		switch(dm_coll[i].type)
		{
			case COLLECTION_FIXED:{
				collection_fixed_t *tmp = (collection_fixed_t *)malloc(sizeof(collection_fixed_t));
				dm_coll->config = (void *)tmp;
				bson_property prop = bson_property_get(collection.pointer, "config");
				fixed_collection_init(prop.pointer, tmp);
				fixed_collection_create(dm_coll);
				break;
			}
			case COLLECTION_VARIABLE:{
				break;
			}
			default:{
				break;
			}
		}
	}
	
	/*
	collection_t *p = dm_collections;
	for (; p!=NULL; p=p->_next){
		printf("type : %d\n", p->type);
		printf("name : %s\n", p->name);
		printf("CONFIG : \n");
		collection_fixed_t *config = (collection_fixed_t *)p->config;
		printf("   block_size : %d\n", config->block_size);
		printf("   offset     : %d\n", config->offset);
		printf("==============\n");
	}
	*/
	memory_instance_delete(mem_index);
}

void collection_config_save(char *config, collection_t *collection)
{
	if (config==NULL)
		config = dm_config_path;
	

	int mem_index = memory_instance_create();
	
	bson_encode_start(mem_index);
		bson_encode_document_start(mem_index, "collections");

		collection_t *i=dm_collections;
		
		if (collection!=NULL)
			i = collection;

		for (i; i!=NULL; i=i->_next){
			bson_encode_document_start(mem_index, "");
				bson_encode_string(mem_index, "name", i->name);
				bson_encode_int32(mem_index, "type", i->type);
				
				bson_encode_document_start(mem_index, "config");
				if (i->type == COLLECTION_FIXED)
					fixed_collection_config_save(mem_index, (collection_fixed_t *)i->config);
				
				bson_encode_document_end(mem_index);
			bson_encode_document_end(mem_index);
		}

		bson_encode_document_end(mem_index);
	bson_encode_end(mem_index);
	
	assert(fileio_write_memory_to_file(mem_index, config)!=-1);
}