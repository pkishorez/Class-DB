/* 
 * File:   lib_regex.h
 * Author: Kishore
 *
 * Created on June 5, 2015, 5:40 PM
 */

#include "lib_types.h"
int bnf_parse_string(char *bnf, char *str, bnf_cap *caps, int no_caps);
int bnf_parse_memory(char *bnf, int mem_index, bnf_mcap *mcaps, int no_caps);