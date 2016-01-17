/**
 * @file lib_websocket.c
 * @author Kishore
 * @date July 29, 2015, 9:58 PM
 * 
 * @brief 
 * 
 * 
 */

#include "headers/headers.h"
#include "headers/lib_json.h"
#include "headers/lib_utilities.h"
#include "headers/lib_bnf.h"
#include "headers/lib_terminal.h"

static int n_nested_brackets = 0;

static int n_properties[100];

static void _json_add_padding(int coma);
static void _json_add_padding(int coma)
{
	if (n_properties[n_nested_brackets]>0 && coma)
		printf(",");
	printf("\n");
	int i=0;
	for (;i<n_nested_brackets+1; i++)
		printf("    ");
}


/**
 * Start encoding a json string.
 */
void json_encode_start()
{
	n_nested_brackets = 0;
	printf("{");
}


/**
 * Start a nested block inside json string.
 */
void json_array_start(char *key)
{
	_json_add_padding(1);
	n_properties[n_nested_brackets]++;
	if (key!=NULL)
		printf("\"%s\" : ", key);
	printf("[");
	n_nested_brackets++;
	n_properties[n_nested_brackets] = 0;
}


/**
 * End nested block.
 */
void json_array_end()
{
	n_nested_brackets--;
	_json_add_padding(0);
	printf("]");
}

/**
 * Start a nested block inside json string.
 */
void json_nested(char *key)
{
	_json_add_padding(1);
	n_properties[n_nested_brackets]++;
	if (key!=NULL && strcmp(key,""))
		printf("\"%s\" : ", key);
	printf("{");
	n_nested_brackets++;
	n_properties[n_nested_brackets] = 0;
}

/**
 * End nested block.
 */
void json_unested()
{
	n_nested_brackets--;
	_json_add_padding(0);
	printf("}");
}

/**
 * Add number property to json.
 * @param key : key of property of json.
 * @param value : value of property of json.
 */
void json_add_number(char *key, long int value)
{
	_json_add_padding(1);
	printf("\"%s\"", key);
	printf(" : ");
	printf("%ld", value);
	n_properties[n_nested_brackets]++;
}


void json_add_string(char *key, char * value)
{
	_json_add_padding(1);
	printf("\"%s\" : \"%s\"", key, value);
	n_properties[n_nested_brackets]++;
}

void json_add_string_noquotes(char *key, char * value)
{
	_json_add_padding(1);
	printf("\"%s\" : %s", key, value);
	n_properties[n_nested_brackets]++;
}

void json_encode_end()
{
	assert(n_nested_brackets==0);
	n_properties[n_nested_brackets] = 0;
	printf("\n}\n");
}



void json_parse(char *json)
{
	bnf_cap caps[10];
	int parsed = 0;
	bnf_rstatus_t status = bnf_parse_string("*{<wspace>\n}<{>*{<wspace>\n}", json+parsed, NULL, 0);
	if (status.status<0)
		printf("Invalid json");
	parsed+=status.status;
	status = bnf_parse_string("\"(*!\")\"*<wspace>:*<wspace>\"(*!\")\"", json+parsed, caps, 10);
	if (status.status<0)
		printf("Invalid json");
	else{
		printf("%.*s  :  %.*s\n", caps[0].len, caps[0].ptr, caps[1].len, caps[1].ptr);
	}
	parsed+=status.status;
}