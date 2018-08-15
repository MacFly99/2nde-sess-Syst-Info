test: test.o matrix.o
	gcc -o test test.o matrix.o

matrix.o: matrix.c
	gcc -o matrix.o -c matrix.c -g -Wall -W -Werror -std=gnu99

test.o: test.c matrix.h
	gcc -o test.o -c test.c -g -Wall -W -Werror -std=gnu99