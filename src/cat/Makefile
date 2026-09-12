CC = gcc
WWW = -Wall -Werror -Wextra

all: s21_cat clean 

s21_cat: s21_cat.o
	$(CC) $(WWW) -o s21_cat s21_cat.o

s21_cat.o: s21_cat.c
	$(CC) $(WWW) -c s21_cat.c

clean:
	rm -rf *.o s21_cat

rebuild: s21_cat
