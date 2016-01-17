/**
 * @file lib_types.h
 * @author Kishore
 * @date October 25, 2015, 1:48 PM 
 */

#ifndef LIB_MODULES_TYPES

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
	
	typedef struct
	{
		int bnf_parsed;
		int status;
	}bnf_rstatus_t;

	typedef struct{
		memory_get ref;
		int len;
	}bnf_mcap;

	enum bnf_errors{
		BNF_ERROR_PARSE					=	-1,
		BNF_ERROR_STRING_INCOMPLETE		=	-2
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
			ansi_error("File : %s :%d: %s\n", __FILE__, __LINE__ , msg);\
			return 0;\
		}


	/**
	 * ===========
	 * BSON MODULE
	 * ===========
	 */

	#define BSON_SINT32 "\x10"
	#define BSON_SINT64 "\x12"
	#define BSON_SSTRING "\x02"
	#define BSON_SDOCUMENT "\x03"
	#define BSON_SBINARY "\x05"
	#define BSON_SUNKNOWN "\x00"

	#define BSON_INT32 '\x10'
	#define BSON_INT64 '\x12'
	#define BSON_STRING '\x02'
	#define BSON_DOCUMENT '\x03'
	#define BSON_BINARY '\x05'
	#define BSON_UNKNOWN '\x00'

	typedef memory_get bson_document;
	
	typedef memory_get bson_array;
	
	typedef struct{
		memory_get pointer;
		int type;
	}bson_property;
	
	/**
	 * =================
	 * DATA MODEL MODULE
	 * =================
	 */
	enum {
		DATATYPE_INT32 = 1,
		DATATYPE_INT64,
		DATATYPE_STRING,
		DATATYPE_BINARY,
		DATATYPE_BSON
	};
	
	
	/**
	 * ===============
	 * TERMINAL MODULE
	 * ===============
	 */
	typedef struct{
		int isspecial;
		int ch;
		int ctrl;
		int shift;
		int alt;
	}keystroke_t;
	
	enum {
		CHAR_TAB = 9,
		CHAR_BACKSPACE = 127,
		CHAR_ENTER = 10,
		CHAR_ESCAPE = 27,
		CHAR_UP = -200,
		CHAR_DOWN,
		CHAR_LEFT,
		CHAR_RIGHT,
		CHAR_HOME,
		CHAR_END,
		CHAR_F2,
		CHAR_F3,
		CHAR_DELETE
	};
	
	/**
	 * ====================
	 * BNF_EXTENDED LIBRARY
	 * ====================
	 */
	typedef struct bnfextended_struct{
		char *common;
		char *decision;
		int isend;
		struct bnfextended_struct **next;
	}bnfextended_t;
	
	typedef struct bnfextended_pointer{
		char* str;
		struct bnfextended_pointer *next;
	}bnfextended_pointer_t;

	enum bnfextended_status{
		BNFEXTENDED_COMPLETE,
		BNFEXTENDED_SUGGESTION_NOT_FOUND,
		BNFEXTENDED_SUGGESTION_FOUND,
		BNFEXTENDED_EXCEEDED
	};

	/**
	 * ===========================
	 * DATA MODEL PROPERTY LIBRARY
	 * ===========================
	 */
	
	enum property_types{
		DM_PROPERTY_INT32 = 1,
		DM_PROPERTY_INT64 = 2,
		DM_PROPERTY_NUMBER = 3,
		DM_PROPERTY_STRING = 4,
		DM_PROPERTY_BINARY = 5
	};
	typedef struct {
		enum property_types type;
		int offset;
		int length;
	}dm_property_t;

	#define LIB_MODULES_TYPES 1
#endif