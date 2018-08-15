all: matrix.h matrix.c    
  gcc matrix.c -o test -g -Wall -W -Werror -std=gnu99 matrix.h
