CC = gcc
WWW = -Wall -Werror -Wextra

all: s21_grep clean

s21_grep: s21_grep.o
	$(CC) $(WWW) -o s21_grep s21_grep.o

s21_grep.o: s21_grep.c
	$(CC) $(WWW) -c s21_grep.c

clean:
	rm -rf *.o s21_grep

rebuild: s21_grep