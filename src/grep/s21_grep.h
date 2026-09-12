#ifndef S21_GREP_H
#define S21_GREP_H

#include <getopt.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  int e;
  int i;
  int v;
  int c;
  int l;
  int n;
  int h;
  int s;
  int f;
  int o;
  char *pattern;
  int len_pattern;
  int memory_pattern;
  int error_flag;
} Flags;

Flags parse_flags(int argc, char *argv[], Flags *fl);
void add_pattern(Flags *fl, char *pattern);
void add_patt_from_file(Flags *fl, char *file);
int process_file(Flags fl, char *path, regex_t *reg);
void print_match(Flags fl, regex_t *reg, char *line, int line_cnt, char *file);
void match_flags(Flags fl, int line_cnt, char *file);
int output(Flags fl, int argc, char **argv);
void output_line(char *line);
void finalize_output(Flags fl, int match_cnt, char *file);

#endif
