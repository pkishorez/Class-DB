/* 
 * @file lib_utilities.h
 * @author Kishore
 * @date June 7, 2015, 12:14 PM
 * 
 * @brief Easy to use functions used in Class-DB project.
 */

#define assert_msg(expr, msg) \
	if (!(expr))\
	{\
		printf("File : %s :%d: %s\n", __FILE__, __LINE__ , msg);\
		return 0;\
	}

int U_streq(char *s1, char *s2);