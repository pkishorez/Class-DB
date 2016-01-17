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

#include "headers/headers.h"
#include "headers/lib_utilities.h"
#include "headers/lib_terminal.h"

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



/**
 * Circular left rotate of 16 bit integer.
 * @param num : number that should be rotated.
 * @param k : number of bits that should be rotated.
 * @returns the number rotated k bits.
 * 
 * The number given in parameter and returned are both
 * of type uint16_t. It left rotates the bits and returns
 * the result.
 */
uint16_t lrotate16(uint16_t num, int k)
{
	return (num<<k) | (num>>(16-k));
}



/**
 * Circular left rotate of 32 bit integer.
 * @param num : number that should be rotated.
 * @param k : number of bits that should be rotated.
 * @returns the number rotated k bits.
 * 
 * The number given in parameter and returned are both
 * of type uint32_t. It left rotates the bits and returns
 * the result.
 */
uint32_t lrotate32(uint32_t num, int k)
{
	//num = to_bigendian32(num);
	return num = (num<<k) | (num>>(32-k));
	//return to_bigendian32(num);
}



/**
 * Circular left rotate of 64 bit integer.
 * @param num : number that should be rotated.
 * @param k : number of bits that should be rotated.
 * @returns the number rotated k bits.
 * 
 * The number given in parameter and returned are both
 * of type uint64_t. It left rotates the bits and returns
 * the result.
 */
uint64_t lrotate64(uint64_t num, int k)
{
	return (num<<k) | (num>>(64-k));
}



/**
 * Check if the system is little endian.
 * @returns 1 if little endian and 0 if it is not.
 */
int is_little_endian()
{
	unsigned int i = 1;
	return (int)*((char*)&i);
}



/**
 * Check if the system is big endian.
 * @returns 1 if big endian and 0 if it is not.
 * 
 * This function is just the negation of `is_little_endian()` function.
 */
int is_big_endian()
{
	return !is_little_endian();
}



/**
 * Converts 64bit little endian number `num` to big endian.
 * @param num : number that should be converted to big endian.
 * @returns number that is converted to big endian.
 * 
 * Note that conversion happens only if the system is little endian.
 * In case of Big endian system this function returns the same
 * number without any modification.
 */
uint64_t to_bigendian64(uint64_t num)
{
	if (is_little_endian())
	{
		char *ch = (char *)&num;
		int i=0;
		for (i=0; i<=3; i++)
		{
			int tmp = ch[i];
			ch[i] = ch[7-i];
			ch[7-i] = tmp;
		}
		return num;
	}
	else
		return num;
}



/**
 * Converts 32bit little endian number `num` to big endian.
 * @param num : number that should be converted to big endian.
 * @returns number that is converted to big endian.
 *
 * Note that conversion happens only if the system is little endian.
 * In case of Big endian system this function returns the same
 * number without any modification.
 */
uint32_t to_bigendian32(uint32_t num)
{
	if (is_little_endian())
	{
		char *ch = (char *)&num;
		int i=0;
		for (i=0; i<=1; i++)
		{
			int tmp = ch[i];
			ch[i] = ch[3-i];
			ch[3-i] = tmp;
		}
		return num;
	}
	else
		return num;
}



/**
 * Converts 16bit little endian number `num` to big endian.
 * @param num : number that should be converted to big endian.
 * @returns number that is converted to big endian.
 * 
 * Note that conversion happens only if the system is little endian.
 * In case of Big endian system this function returns the same
 * number without any modification.
 */
uint16_t bigendian16(uint16_t num)
{
	if (is_little_endian())
	{
		char *ch = (char *)&num;
		int i=0;
		for (i=0; i<=0; i++)
		{
			int tmp = ch[i];
			ch[i] = ch[1-i];
			ch[1-i] = tmp;
		}
		return num;
	}
	else
		return num;
}

void kprintf(char *str)
{
	int i=0;
	for (i=0; str[i]!='\0'; i++)
	{
		if (isprint(str[i]) || str[i]=='\n')
			printf("%c", str[i]);
		else
			ansi_bold_text("%d", str[i]);
	}
}