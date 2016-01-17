/**
 * @file dm_fixed_collection.c
 * @author Kishore
 * @date November 23, 2015, 6:02 PM
 * 
 * @brief 
 * 
 * {
 *		"offset" : {offset},
 *		"block_size" : {block_size}
 * }
 */

#include <string.h>

#include "../headers/dm_types.h"
#include "../../libraries/headers/headers.h"
#include "../../libraries/headers/lib_fileio.h"
#include "../../libraries/headers/lib_terminal.h"
extern const char dm_data_path[];

void fixed_collection_init(bson_document collection, collection_fixed_t *config)
{
	config->block_size = bson_get_int32(collection, "block_size");
	config->offset = bson_get_int32(collection, "offset");
}


void fixed_collection_create(collection_t *config)
{
	collection_fixed_t *fconfig = (collection_fixed_t *)(config->config);
	printf("%d\n", fconfig->block_size);

	char collection[256];
	strcpy(collection, dm_data_path);
	strcat(collection, config->name);
	strcat(collection, ".db");

	if (fileio_exists(collection)){
		config->map = fileio_map_file(collection);
		fconfig->n_blocks = (fileio_size(collection)-fconfig->offset)/fconfig->block_size;
	}
	else{
		int n_blocks = (COLLECTION_PREALLOC_FILESIZE-fconfig->offset)/fconfig->block_size;
		fconfig->n_blocks = n_blocks;
		int fd = fileio_create(collection, fconfig->offset+fconfig->block_size*fconfig->n_blocks);
		close(fd);
		ansi_bold_text("Successfully created collection : \n");
		ansi_fg_bg_text(C_GREEN, C_DEFAULT, "\tName       : %s\n", config->name);
		ansi_fg_bg_text(C_GREEN, C_DEFAULT, "\tOffset     : %d\n", fconfig->offset);
		ansi_fg_bg_text(C_GREEN, C_DEFAULT, "\tN_Blocks   : %d\n", fconfig->n_blocks);
		ansi_fg_bg_text(C_GREEN, C_DEFAULT, "\tBlock size : %d\n", fconfig->block_size);
		ansi_fg_bg_text(C_GREEN, C_DEFAULT, "\tFile size  : %d\n\n\n", fconfig->offset+fconfig->block_size*fconfig->n_blocks);
		
		
	}
}


void fixed_collection_config_save(int mem_index, collection_fixed_t *config)
{
	bson_encode_int32(mem_index, "offset", config->offset);
	bson_encode_int32(mem_index, "block_size", config->block_size);
}