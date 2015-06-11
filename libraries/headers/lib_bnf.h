/* 
 * File:   lib_regex.h
 * Author: Kishore
 *
 * Created on June 5, 2015, 5:40 PM
 */

typedef struct
{
	char *ptr;
	int len;
}bnf_cap;

int parse_bnf(char *bnf, char *string, bnf_cap *caps, int no_caps);

int parse_terminal(char *bnf_parsed, char *str_parsed);
char * bnf_move_step(char *bnf_parsed);