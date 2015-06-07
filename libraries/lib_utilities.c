/**
 * @file lib_utilities.c
 * @author Kishore
 * @date June 7, 2015, 12:14 PM
 * 
 * @brief Easy to use functions used in Class-DB project.
 * 
 * Utilities library defines all functions and macros that
 * could be useful in the project. Simple yet important and
 * more frequently used functions are stored in utilities library.
 */

#include <stdio.h>
#include <string.h>
#include "headers/lib_utilities.h"

/**
 * Checks if two strings are equal or not.
 * @param s1
 * @param s2
 * @return 1 if equal and 0 otherwise.
 * 
 * Utility_StringEqual(full form) function checks if two
 * strings are equal or not. Strings need not be of equal
 * length. Checking will happen till the length of small
 * string.
 */
int U_streq(char *s1, char *s2)
{
	while (*s1!='\0' && *s2!='\0')
	{
		if (*s1!=*s2)
			return 0;
		s1++;
		s2++;
	}
	return 1;
}