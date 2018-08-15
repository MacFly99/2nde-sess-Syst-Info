all: matrix.h matrix.c    
	gcc test.c -o test -g -Wall -W -Werror -std=gnu99
