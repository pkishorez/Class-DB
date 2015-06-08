/**
 * @file lib_regex.c
 * @author Kishore
 * @date June 5, 2015, 5:38 PM
 * 
 * @brief BNF language parser library.
 * 
 * BNF is the most commonly used grammar for parsing
 * scripting languages like HTML, XML etc. To facilitate
 * the parsing functionality BNF grammar parsing library
 * was written for easy parsing of http requests and responses.
 * 
 * @see http://www.wikipedia.com/bnf
 */

#include "headers/headers.h"
#include "headers/lib_utilities.h"
#include "headers/lib_bnf.h"
#include <ctype.h>

static int parse_char(void);

static char *str_parsed;
static char *bnf_parsed;

/**
 * Parses BNF of given grammar and updates caps array with
 * parsed information.
 * @param bnf : BNF expression
 * @param string : string to parse
 * @param caps : array where result is updated
 * @param no_caps : no of elements in caps array.
 * @returns integer whose value is length of string parsed so far.
 * 
 * The parameter bnf consists of BNF grammar upon which the
 * string is parsed with.
 */
int parse_bnf(char *bnf, char *string, bnf_cap *caps, int no_caps)
{
	int cap_no = 0;
	str_parsed = string;
	bnf_parsed = bnf;

	while (1)
	{
		switch(*bnf_parsed)
		{
			case '(' : {
				assert_msg(cap_no<no_caps, "Number of caps should cope with number of brackets.");
				caps[cap_no].ptr = str_parsed;
				bnf_parsed++;
				break;
			}
			case ')' : {
				caps[cap_no].len = str_parsed - caps[cap_no].ptr;
				cap_no++;
				bnf_parsed++;
				break;
			}
			case '*' : {
				bnf_parsed++;
				while (1)
				{
					if (!parse_char())
						break;
					str_parsed++;
				}
				bnf_parsed = bnf_move_step(bnf_parsed);
				break;
			}
			case '+' : {
				bnf_parsed++;
				if (!parse_char())
					return 0;
				str_parsed++;
				while (1)
				{
					if (!parse_char())
						break;
					str_parsed++;
				}
				bnf_parsed = bnf_move_step(bnf_parsed);
				break;
			}
			case '\0' : {
				return 1;
			}
			default : {
				if (!parse_char())
					return 0;
				bnf_parsed = bnf_move_step(bnf_parsed);
				str_parsed++;
			}
		}
	}
}

static int parse_char(void)
{
	switch(*bnf_parsed)
	{
		case '<' : {
			return parse_nterminal(bnf_parsed, str_parsed);
		}
		case '{' : {
			// Functionality not implemented yet.
			return 0;
		}
		case '(' : {
			// This case should never occur.
			return 0;
		}
		case ')' : {
			// This case should never occur.
			return 0;
		}
		default : {
			if (*bnf_parsed==*str_parsed)
				return 1;
			return 0;
			break;
		}
	}
}

char * bnf_move_step(char *bnf_parsed)
{
	if (*bnf_parsed=='\0')
		return bnf_parsed;

	if (*bnf_parsed=='<')
	{
		if (*(bnf_parsed+1)!='>')
		{
			int end = 0;
			while (*(bnf_parsed+end)!='>' && *(bnf_parsed+end)!='\0')
				end++;
			return bnf_parsed+end+1;
		}
		else{
			if (*(bnf_parsed+2)!='>')
			{
				return NULL;
			}
			return bnf_parsed+3;
		}
	}
	return bnf_parsed+1;
}
int parse_nterminal(char *bnf_parsed, char *str_parsed)
{
	if (*str_parsed=='\0')
		return 0;
	if (U_streq("<digit>", bnf_parsed))
	{
		if (isdigit(*str_parsed))
			return 1;
		return 0;
	}
	else if (U_streq("<lcase>", bnf_parsed))
	{
		if (islower(*str_parsed))
			return 1;
		return 0;
	}
	else if (U_streq("<ucase>", bnf_parsed))
	{
		if (isupper(*str_parsed))
			return 1;
		return 0;
	}
	else if (U_streq("<alpha>", bnf_parsed))
	{
		if (isalpha(*str_parsed))
			return 1;
		return 0;
	}
	else if (U_streq("<alpha_>", bnf_parsed))
	{
		if (isalpha(*str_parsed) || (*str_parsed=='_'))
			return 1;
		return 0;
	}
	else if (U_streq("<alnum>", bnf_parsed))
	{
		if (isalnum(*str_parsed))
			return 1;
		return 0;
	}
	else if (U_streq("<alnum_>", bnf_parsed))
	{
		if (isalnum(*str_parsed) || (*str_parsed=='_'))
			return 1;
		return 0;
	}
	else if (U_streq("<wspace>", bnf_parsed))
	{
		if ((*str_parsed==' ') || (*str_parsed=='\t'))
			return 1;
		return 0;
	}
	else if (U_streq("<<>", bnf_parsed))
	{
		if (*str_parsed=='<')
			return 1;
		return 0;
	}
	else if (U_streq("<>>", bnf_parsed))
	{
		if (*str_parsed=='>')
			return 1;
		return 0;
	}
	else if (U_streq("<{>", bnf_parsed))
	{
		if (*str_parsed=='{')
			return 1;
		return 0;
	}
	else if (U_streq("<}>", bnf_parsed))
	{
		if (*str_parsed=='}')
			return 1;
		return 0;
	}
	else
	{
		// This is not a non terminal.
		return 0;
	}
}