/**
 * @file lib_types.h
 * @author Kishore
 * @date October 25, 2015, 1:48 PM 
 */

#ifndef MODULES_TYPES

	/**
	 * =============
	 * MEMORY MODULE
	 * =============
	 */
	// Default values of some important constants used in this library.
	#define MEM_BLOCK_SIZE 256
	#define N_MEM_BLOCKS 1000

	typedef struct{
		char cdata[MEM_BLOCK_SIZE];
		int __next_block;
		int __filled;
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


	typedef struct{
		int _instance_index;
		int _block_index;
		int _index;
	}memory_get;


	/**
	 * ==========
	 * BNF MODULE
	 * ==========
	 */
	typedef struct
	{
		char *ptr;
		int len;
		int _index;
	}bnf_cap;

	typedef struct{
		memory_get ref;
		int len;
	}bnf_mcap;

	enum bnf_errors{
		ERROR_PARSE					=	-1,
		ERROR_STRING_INCOMPLETE		=	-2
	};

	/**
	 * =================================
	 * EPOLL MEMORY SOCKET COMBO LIBRARY
	 * =================================
	 */
	enum ems_errors {
		EMS_ERROR_ACCEPT_CLIENT = -1,
		EMS_ERROR_MEMORY_CREATE = -2,
		EMS_ERROR_EPOLL_ADD = -3
	};

	/**
	 * ================
	 * TERMINAL LIBRARY
	 * ================
	 */
	enum colors {
		C_BLACK,
		C_RED,
		C_GREEN,
		C_YELLOW,
		C_BLUE,
		C_MAGENTA,
		C_CYAN,
		C_WHITE,
		C_DEFAULT = 9
	};

	enum params {
		A_BOLD = 1u << 0,
	};


	/**
	 * ===============
	 * REQUESTS MODULE
	 * ===============
	 */
	#define MAX_REQUEST_CONNECTIONS 10000

	struct epoll_member{
		int fd;
		int mem_instance;
		void *addon;

		int _next_free_instance;
	};

	/**
	 * ===========
	 * BSON MODULE
	 * ===========
	 */
	#define MAX_SUB_DOCUMENTS 10

	/**
	 * ================
	 * UTILITIES MODULE
	 * ================
	 */
	#define assert_msg(expr, msg) \
		if (!(expr))\
		{\
			printf("File : %s :%d: %s\n", __FILE__, __LINE__ , msg);\
			return 0;\
		}


	#define MODULES_TYPES 1
#endif