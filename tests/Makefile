## -*- Makefile -*-
##
## Author: kishore
## Time: Jun 7, 2015 4:21:06 PM
##
## Makefile for defining rules to run tests of Class-DB.
## Make sure that project was run and ClassDB.a file was
## generated in bin folder, and then run this makefile to
## test updated libraries.

CC = gcc

TESTS_C = $(wildcard test_*.c)
TESTS_O = $(foreach lib, $(TESTS_C), $(lib:%.c=%.o))

test_%.o : test_%.c test_%.h
	gcc -c -g $<

test : tests.c tests.a ../bin/ClassDB.a
	gcc -o test seatest/seatest.c $^

tests.a : $(TESTS_O)
	ar r $@ $^
	-rm *.o