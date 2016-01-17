/**
 * @file lib_bnfextended.h
 * @author Kishore
 * @date January 5, 2016, 5:38 PM 
 */

#include "lib_types.h"


bnfextended_t* bnfextended_compile(bnfextended_pointer_t *pointer);
void bnfextended_print(bnfextended_t *start, int nest);
bnfextended_pointer_t* bnfextended_add(bnfextended_pointer_t *p, char *bnf);
int bnfextended_suggest(bnfextended_t *start, char *input, int terminal);