/**
 * @file test_bnf.c
 * @author Kishore
 * @date June 7, 2015, 2:48 PM
 * 
 * @brief Unit testing for BNF library
 */

#include "../libraries/headers/headers.h"
#include "../libraries/headers/lib_bnf.h"
#include "http_parser.h"
#include "seatest/seatest.h"

void bnf_terminal_tests();
void bnf_terminal_tests()
{
	// DIGIT NON TERMINAL
	char i;
	for (i='0'; i<='9'; i++){
		char ch[2]="";
		ch[0] = i;
		ch[1] = '\0';
		assert_true(parse_terminal("<digit>", ch));
		assert_true(parse_terminal("<alnum>", ch));
		assert_true(parse_terminal("<alnum_>", ch));
	}
	for (i='a'; i<='z'; i++){
		char ch[2]="";
		ch[0] = i;
		ch[1] = '\0';
		assert_true(parse_terminal("<lcase>", ch));
		assert_true(parse_terminal("<alpha>", ch));
		assert_true(parse_terminal("<alnum>", ch));
		assert_true(parse_terminal("<alnum_>", ch));
		assert_true(parse_terminal("<alpha_>", ch));
	}
	for (i='A'; i<='Z'; i++){
		char ch[2]="";
		ch[0] = i;
		ch[1] = '\0';
		assert_true(parse_terminal("<ucase>", ch));
		assert_true(parse_terminal("<alpha>", ch));
		assert_true(parse_terminal("<alnum>", ch));
		assert_true(parse_terminal("<alnum_>", ch));
		assert_true(parse_terminal("<alpha_>", ch));
	}
	
	assert_true(parse_terminal("<<>", "<"));
	assert_true(parse_terminal("<>>", ">"));
	assert_true(parse_terminal("<{>", "{"));
}

void bnf_move_step_tests();
void bnf_move_step_tests()
{
	assert_string_equal(bnf_move_step("<<>"), "");
	assert_string_equal(bnf_move_step("<>>"), "");
	assert_string_equal(bnf_move_step("<equal>"), "");
	assert_string_equal(bnf_move_step("<lcase>"), "");
	assert_string_equal(bnf_move_step("<k><l>"), "<l>");
	assert_string_equal(bnf_move_step("<kishore><kittu>"), "<kittu>");
	assert_string_equal(bnf_move_step("k<kishore>"), "<kishore>");
}

void parse_bnf_tests();
void parse_bnf_tests()
{
	assert_true(parse_bnf("<digit>", "123", NULL, 0));
	assert_true(parse_bnf("<digit><digit><digit><digit>", "123", NULL, 0)==ERROR_STRING_INCOMPLETE);
	assert_true(parse_bnf("<digit><digit><digit>", "123", NULL, 0));
	assert_true(parse_bnf("<digit><alpha><lcase><ucase>", "1AbC", NULL, 0));
	assert_true(parse_bnf("<digit>abc<alpha>def<lcase><ucase>", "1abcAdefbC", NULL, 0));

	bnf_cap caps[5];
	parse_bnf("<digit>(abc)<alpha>def(<lcase><ucase>)", "1abcAdefbC", caps, 5);
	assert_string_starts_with("abc", caps[0].ptr);
	assert_string_starts_with("bC", caps[1].ptr);
	assert_int_equal(3, caps[0].len);
	assert_int_equal(2, caps[1].len);
	
	assert_true(parse_bnf("(*<digit>)(abc)<alpha>def(<lcase><ucase>)", "abcAdefbC", caps, 5));
	
	assert_true(parse_bnf("(+<digit>)(abc)<alpha>def(<lcase><ucase>)", "abcAdefbC", caps, 5)==ERROR_PARSE);
	
	char *variable = "<alpha_>*<alnum_>";
	assert_true(parse_bnf(variable, "_kishore", NULL, 0));
	assert_true(parse_bnf(variable, "_kishore_123", NULL, 0));
	assert_true(parse_bnf(variable, "kishore123", NULL, 0));
	assert_true(parse_bnf(variable, "123kishore", NULL, 0)==ERROR_PARSE);
	assert_true	(parse_bnf(variable, "__kishore", NULL, 0));
	assert_true(parse_bnf(variable, "kishore___123", NULL, 0));
	
	
	assert_true(parse_bnf("!kkishore", "ikishore", NULL, 0));
	assert_true(parse_bnf("!<wspace>kishore", " ishore", NULL, 0)==ERROR_PARSE);
	
	
	// Add tests for compound terminal.
	assert_true(parse_bnf("{kishore}", "a", NULL, 0)==ERROR_PARSE);
	assert_true(parse_bnf("{kishore}", "k", NULL, 0));
	assert_true(parse_bnf("(*{kishore})(*{kittu})", "kishorekittu", caps, 2));
	assert_true(parse_bnf("(*{abcd})(*!{abcd})", "abcde!234agh!123", caps, 2));
}

void test_http_protocol();
void test_http_protocol()
{
	int count = 0;
	for (count=0; requests[count].name; count++)
	{
		printf("Testing %d\n\n", count);
		if (requests[count].type != HTTP_REQUEST)
			continue;
		char bnf_http_status[] = "*<wspace>(+<alpha>) (*!<wspace>) HTTP/(+<digit>).(+<digit>)\r\n";
		char bnf_http_header[] = "*<wspace>(*!<wspace>)*<wspace>:*<wspace>(+!\r)\r\n";
		bnf_cap caps[10];
		char *raw = (char *)requests[count].raw;
		char *request_url = (char *)requests[count].request_url;
		int status = parse_bnf(bnf_http_status, raw, caps, 10);

		int major = atoi(caps[2].ptr);
		int minor = atoi(caps[3].ptr);
		assert_true(status>0);

		assert_string_starts_with(request_url, caps[1].ptr);
		assert_true(requests[count].http_major==major);
		assert_true(requests[count].http_minor==minor);
	}
}
void test_bnf()
{
	test_fixture_start();
	run_test(bnf_terminal_tests);
	run_test(bnf_move_step_tests);
	run_test(parse_bnf_tests);
	run_test(test_http_protocol);
	test_fixture_end();
}