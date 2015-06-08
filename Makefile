## -*- Makefile -*-
##
## Author: kishore
## Time: Jun 7, 2015 4:21:06 PM
## 
## Makefile for defining rules to compile the Class-DB DBMS


# List of all library source files.
LIBRARIES_C = $(wildcard libraries/lib_*.c)

# List of all object files of corresponding source files.
LIBRARIES_O = $(foreach lib, $(LIBRARIES_C), $(lib:%.c=%.o))



libraries/lib_%.o : libraries/lib_%.c
	gcc -c -g -o $@ $<

ClassDB.a : $(LIBRARIES_O)
	ar r bin/$@ $^
	rm $^