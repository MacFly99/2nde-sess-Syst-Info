all: matrix.c exec

matrix.o:
  gcc -g -Wall -W -Werror -std=gnu99
  
exec: test.c matrix.c
  gcc -g -Wall -W -Werror -std=gnu99
