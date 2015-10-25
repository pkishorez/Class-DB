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
#include "headers/lib_memory.h"
#include "headers/lib_bnf.h"
#include <ctype.h>

static int parse_char(void);

static char *bnf_parsed;

static char *str_parsed;
static int str_parsed_len;

static int is_memory_module;
static memory_get mem_get;

/*
 * ==========================
 * STATIC FUNCTION PROTOTYPES
 * ==========================
 */
static int get_char();
static void move_char();
static int parse_bnf(char *bnf, bnf_cap *caps, bnf_mcap *mcaps, int no_caps);
static char * bnf_move_step(char *bnf_parsed);
static int parse_char(void);
static int parse_terminal(char *bnf_parsed);

static int get_char()
{
	if (is_memory_module){
		int ch = memory_instance_get_char(mem_get);
		return ch;
	}
	else{
		if ((*str_parsed) == '\0')
			return -1;
		return (int)(*str_parsed);
	}
}

static void move_char()
{
	str_parsed_len++;
	if (is_memory_module){
		memory_instance_get_and_move_char(&mem_get);
	}
	else{
		str_parsed++;
	}
}


/**
 * Parse bnf string on memory instance and return results in mcaps.
 * @param bnf : backus normal form of a rule.
 * @param mem_index : Memory instance where string is stored.
 * @param mcaps : array where the results of parsing are stored.
 * @param no_caps : number of caps that mcaps can hold.
 * @returns amount of string parsed.
 */
int bnf_parse_memory(char *bnf, int mem_index, bnf_mcap *mcaps, int no_caps)
{
	is_memory_module = 1;
	mem_get = memory_instance_get(mem_index, 0);
	return parse_bnf(bnf, NULL, mcaps, no_caps);
}


/**
 * Parse bnf string on string and return results in caps.
 * @param bnf : backus normal form of a rule.
 * @param str : string on which bnf is parsed.
 * @param caps : array where the results are stored.
 * @param no_caps : number of caps that caps array can store.
 * @returns amount of string parsed.
 */
int bnf_parse_string(char *bnf, char *str, bnf_cap *caps, int no_caps)
{
	is_memory_module = 0;
	str_parsed = str;
	return parse_bnf(bnf, caps, NULL, no_caps);
}


/**
 * Parses BNF of given grammar and updates caps array with
 * parsed information.
 * @param bnf : BNF expression
 * @param caps : array where result is updated
 * @param mcaps : memory module cap where result is updated.
 * @param no_caps : no of elements in caps array.
 * @returns integer whose value is length of string parsed so far.
 * 
 * The parameter bnf consists of BNF grammar upon which the
 * string is parsed with.
 */
static int parse_bnf(char *bnf, bnf_cap *caps, bnf_mcap *mcaps, int no_caps)
{
	int cap_no = 0;
	bnf_parsed = bnf;
	str_parsed_len = 0;

	while (1)
	{
		switch(*bnf_parsed)
		{
			case '(' : {
				assert_msg(cap_no<no_caps, "Number of caps should cope with number of brackets.");
				if (!is_memory_module){
					caps[cap_no].ptr = str_parsed;
					caps[cap_no].len = str_parsed_len;
				}
				else{
					mcaps[cap_no].ref = mem_get;
					mcaps[cap_no].len = str_parsed_len;
				}
				bnf_parsed++;
				break;
			}
			case ')' : {
				if (!is_memory_module)
					caps[cap_no].len = str_parsed_len - caps[cap_no].len;
				else
					mcaps[cap_no].len = str_parsed_len - mcaps[cap_no].len;
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
					move_char();
				}
				bnf_parsed = bnf_move_step(bnf_parsed);
				break;
			}
			case '+' : {
				bnf_parsed++;
				if (!parse_char()){
					if (get_char()==-1)
						return ERROR_STRING_INCOMPLETE;
					return ERROR_PARSE;
				}
				move_char();
				while (1)
				{
					if (!parse_char())
						break;
					move_char();
				}
				bnf_parsed = bnf_move_step(bnf_parsed);
				break;
			}
			case '\0' : {
				return str_parsed_len;
			}
			default : {
				if (!parse_char()){
					if (get_char()==-1)
						return ERROR_STRING_INCOMPLETE;
					return ERROR_PARSE;
				}
				bnf_parsed = bnf_move_step(bnf_parsed);
				move_char();
			}
		}
	}
}

static int parse_char(void)
{
	if (get_char() == -1)
		return 0;
	switch(*bnf_parsed)
	{
		/**
		 * !<item>
		 * parses if string is not <item> at the place of grammar.
		 * where <item> is any valid terminal or non terminal or a
		 * compound terminal.
		 * 
		 * Ex : "!<wspace>"
		 *      passes : "k", "i", "ksoijda", "!ksoi", "2344!" etc
		 *      fails  : " k", "\tkishore", " 123" etc
		 */
		case '!' : {
			bnf_parsed++;
			int status = parse_char();
			bnf_parsed--;
			return !status;
		}
		/**
		 * terminals bounded in {} constitutes single unit which parses
		 * string if it includes any of the terminal inside {}, and do
		 * not parses if none of the terminals in {} equals.
		 * 
		 * ! within {} is not allowed.
		 * Also character class within character class is also not allowed.
		 * Every `{` should follow `}`. Failure to have it means, BNF grammar
		 * error.
         */
		case '{' : {
			static int checking = 0;
			assert_msg(!checking, "{ within {} is not allowed");
			checking = 1;

			char *tmp_bnf = bnf_parsed + 1;

			while (1)
			{
				assert_msg(*tmp_bnf!='{' , "{ within {} is not allowed");
				assert_msg(*tmp_bnf!='!' , "! within {} is not allowed");
				assert_msg(*tmp_bnf!='\0' , "{ should follow }.");

				if (*tmp_bnf=='}'){
					checking = 0;
					return 0;
				}
				if (parse_terminal(tmp_bnf))
				{
					checking = 0;
					return 1;
				}
				tmp_bnf++;
			}
		}
		default : {
			return parse_terminal(bnf_parsed);
		}
	}
}

static char * bnf_move_step(char *bnf_parsed)
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
	else if (*bnf_parsed=='!'){
		return bnf_move_step(bnf_parsed+1);
	}
	else if (*bnf_parsed=='{')
	{
		int end = 0;
		while (*(bnf_parsed+end)!='}' && *(bnf_parsed+end)!='\0')
			end++;
		return bnf_parsed+end+1;
	}
	return bnf_parsed+1;
}

static int parse_terminal(char *bnf_parsed)
{
	if (get_char()==-1)
		return 0;
	if (U_streq("<digit>", bnf_parsed))
	{
		if (isdigit(get_char()))
			return 1;
		return 0;
	}
	else if (U_streq("<lcase>", bnf_parsed))
	{
		if (islower(get_char()))
			return 1;
		return 0;
	}
	else if (U_streq("<ucase>", bnf_parsed))
	{
		if (isupper(get_char()))
			return 1;
		return 0;
	}
	else if (U_streq("<alpha>", bnf_parsed))
	{
		if (isalpha(get_char()))
			return 1;
		return 0;
	}
	else if (U_streq("<alpha_>", bnf_parsed))
	{
		if (isalpha(get_char()) || (get_char()=='_'))
			return 1;
		return 0;
	}
	else if (U_streq("<alnum>", bnf_parsed))
	{
		if (isalnum(get_char()))
			return 1;
		return 0;
	}
	else if (U_streq("<alnum_>", bnf_parsed))
	{
		if (isalnum(get_char()) || (get_char()=='_'))
			return 1;
		return 0;
	}
	else if (U_streq("<wspace>", bnf_parsed))
	{
		if ((get_char()==' ') || (get_char()=='\t'))
			return 1;
		return 0;
	}
	else if (U_streq("<separator>", bnf_parsed))
	{
		if ((get_char()==' ') || (get_char()=='\t') || (get_char()==':') || (get_char()=='\r') || (get_char()=='\n'))
			return 1;
		return 0;
	}
	else if (U_streq("<!>", bnf_parsed))
	{
		if (get_char()=='!')
			return 1;
		return 0;
	}
	else if (U_streq("<<>", bnf_parsed))
	{
		if (get_char()=='<')
			return 1;
		return 0;
	}
	else if (U_streq("<>>", bnf_parsed))
	{
		if (get_char()=='>')
			return 1;
		return 0;
	}
	else if (U_streq("<{>", bnf_parsed))
	{
		if (get_char()=='{')
			return 1;
		return 0;
	}
	else if (U_streq("<}>", bnf_parsed))
	{
		if (get_char()=='}')
			return 1;
		return 0;
	}
	else if (U_streq("<(>", bnf_parsed))
	{
		if (get_char()=='(')
			return 1;
		return 0;
	}
	else if (U_streq("<)>", bnf_parsed))
	{
		if (get_char()==')')
			return 1;
		return 0;
	}
	else
	{
		assert_msg(*bnf_parsed!='<', "Undefined token defined <.");

		if (*bnf_parsed==get_char())
			return 1;
		return 0;
	}
}