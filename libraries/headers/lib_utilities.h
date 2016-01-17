/* 
 * @file lib_utilities.h
 * @author Kishore
 * @date June 7, 2015, 12:14 PM
 * 
 * @brief Easy to use functions used in Class-DB project.
 */

#include "lib_types.h"
int U_streq(char *s1, char *s2);

uint16_t lrotate16(uint16_t num, int k);
uint32_t lrotate32(uint32_t num, int k);
uint64_t lrotate64(uint64_t num, int k);

int is_little_endian();
int is_big_endian();

uint16_t to_bigendian16(uint16_t num);
uint32_t to_bigendian32(uint32_t num);
uint64_t to_bigendian64(uint64_t num);

void kprintf(char *str);