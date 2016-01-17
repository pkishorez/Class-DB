/**
 * @file dm_types.h
 * @author Kishore
 * @date November 30, 2015, 3:47 PM 
 */

#include "../../libraries/headers/lib_types.h"

#ifndef DM_MODULES_TYPES

	#define COLLECTION_NAME_LEN 200
	#define COLLECTION_PREALLOC_FILESIZE 1024*1024*100 // 100 MB

	typedef struct{
		char name[COLLECTION_NAME_LEN+1];
		int type;
		void* map;
		void* config;
		void* _next;
	}collection_t;

	enum collection_types{
		COLLECTION_FIXED,
		COLLECTION_VARIABLE
	};

	typedef struct{
		int offset;
		int block_size;
		int n_blocks;
		/**Extra properties are defined yet.*/
	}collection_fixed_t;

	typedef struct{
		char name[100];
		int size;
		int offset;
		int n_properties;
	}fixed_single_property_t;
	#define DM_MODULES_TYPES 1
#endif