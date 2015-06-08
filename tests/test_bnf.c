/**
 * @file test_bnf.c
 * @author Kishore
 * @date June 7, 2015, 2:48 PM
 * 
 * @brief Unit testing for BNF library
 */

#include "../libraries/headers/headers.h"
#include "../libraries/headers/lib_bnf.h"
#include "seatest/seatest.h"

void bnf_nterminal_tests();
void bnf_nterminal_tests()
{
	// DIGIT NON TERMINAL
	char i;
	for (i='0'; i<='9'; i++){
		char ch[2]="";
		ch[0] = i;
		ch[1] = '\0';
		assert_true(parse_nterminal("<digit>", ch));
		assert_true(parse_nterminal("<alnum>", ch));
		assert_true(parse_nterminal("<alnum_>", ch));
	}
	for (i='a'; i<='z'; i++){
		char ch[2]="";
		ch[0] = i;
		ch[1] = '\0';
		assert_true(parse_nterminal("<lcase>", ch));
		assert_true(parse_nterminal("<alpha>", ch));
		assert_true(parse_nterminal("<alnum>", ch));
		assert_true(parse_nterminal("<alnum_>", ch));
		assert_true(parse_nterminal("<alpha_>", ch));
	}
	for (i='A'; i<='Z'; i++){
		char ch[2]="";
		ch[0] = i;
		ch[1] = '\0';
		assert_true(parse_nterminal("<ucase>", ch));
		assert_true(parse_nterminal("<alpha>", ch));
		assert_true(parse_nterminal("<alnum>", ch));
		assert_true(parse_nterminal("<alnum_>", ch));
		assert_true(parse_nterminal("<alpha_>", ch));
	}
	
	assert_true(parse_nterminal("<<>", "<"));
	assert_true(parse_nterminal("<>>", ">"));
	assert_true(parse_nterminal("<{>", "{"));
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
	assert_false(parse_bnf("<digit><digit><digit><digit>", "123", NULL, 0));
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
	
	assert_false(parse_bnf("(+<digit>)(abc)<alpha>def(<lcase><ucase>)", "abcAdefbC", caps, 5));
	
	char *variable = "<alpha_>*<alnum_>";
	assert_true(parse_bnf(variable, "_kishore", NULL, 0));
	assert_true(parse_bnf(variable, "_kishore_123", NULL, 0));
	assert_true(parse_bnf(variable, "kishore123", NULL, 0));
	assert_false(parse_bnf(variable, "123kishore", NULL, 0));
	assert_true	(parse_bnf(variable, "__kishore", NULL, 0));
	assert_true(parse_bnf(variable, "kishore___123", NULL, 0));

}
void test_bnf()
{
	test_fixture_start();
	run_test(bnf_nterminal_tests);
	run_test(bnf_move_step_tests);
	run_test(parse_bnf_tests);
	test_fixture_end();
}