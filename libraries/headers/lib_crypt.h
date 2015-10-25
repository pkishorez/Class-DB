/**
 * @file lib_crypt.h
 * @author Kishore
 * @date July 3, 2015, 1:28 PM 
 */

#include "lib_types.h"
char* sha1(char *msg);
unsigned char* bsha1(char *msg);

void base64(unsigned char *input, unsigned char *output);
void base64len(unsigned char *input, unsigned char *output, int len);