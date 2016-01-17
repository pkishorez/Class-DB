/**
 * @file lib_json.h
 * @author Kishore
 * @date July 29, 2015, 9:58 PM 
 */

#include "lib_types.h"
void json_encode_start();
void json_nested(char *key);
void json_unested();
void json_array_start(char *key);
void json_array_end();
void json_add_number(char *key, long int value);
void json_add_string(char *key, char * value);
void json_add_string_noquotes(char *key, char * value);
void json_encode_end();
void json_parse(char *json);