/* 
 * File:   lib_regex.h
 * Author: Kishore
 *
 * Created on June 5, 2015, 5:40 PM
 */

#include "lib_types.h"
bnf_rstatus_t bnf_parse_string(char *bnf, char *str, bnf_cap *caps, int no_caps);
bnf_rstatus_t bnf_parse_memory(char *bnf, int mem_index, bnf_mcap *mcaps, int no_caps);
bnf_rstatus_t bnf_parse_memory_t(char *bnf, int mem_index, int offset, bnf_mcap *mcaps, int no_caps);