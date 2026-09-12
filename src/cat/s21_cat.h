#ifndef S21_CAT_H
#define S21_CAT_H

#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>

const char *short_option = "beEnstTv";

const struct option long_option[] = {
    {"number-nonblank", no_argument, NULL, 'b'},
    {"number", no_argument, NULL, 'n'},
    {"squeeze-blank", no_argument, NULL, 's'},
    {0, 0, 0, 0}};

typedef struct {
  int b;
  int e;
  int n;
  int s;
  int t;
  int v;
  int error;
} Flags;

int parse_flags(int argc, char *argv[], Flags *fl);
FILE *print_file(FILE *fp, Flags *fl);

#endif