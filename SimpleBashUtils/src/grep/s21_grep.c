#include "s21_grep.h"

#define BUFFER_SIZE 1024

int main(int argc, char *argv[]) {
  Flags fl = {0};
  fl.error_flag = 0;  // Изначально флаг ошибок выключен
  fl = parse_flags(argc, argv, &fl);

  if (fl.error_flag) {
    fprintf(stderr, "%s", "One or more errors occurred. Exiting.\n");
    free(fl.pattern);
    fl.error_flag = 1;  // Возвращаем код ошибки
  }

  // if (fl.len_pattern == 0) {
  //   fprintf(stderr, "%s", "Error: no pattern specified\n");
  //   free(fl.pattern);
  //   fl.error_flag = 1;  // Завершение программы с ошибкой
  // }

  int result = output(fl, argc, argv);
  free(fl.pattern);
  return result;  // Возвращаем код успеха или ошибки
}

int output(Flags fl, int argc, char **argv) {
  if (fl.len_pattern == 0) {
    fprintf(stderr, "%s", "Error: no pattern specified\n");
    fl.error_flag = 1;  // Установка флага ошибки
  } else {
    regex_t re;
    int rez = regcomp(&re, fl.pattern, REG_EXTENDED | fl.i);

    if (rez) {
      fprintf(stderr, "%s", "Error: regex compilation failed\n");
      fl.error_flag = 1;  // Установка флага ошибки
    } else {
      // Обработка файлов
      for (int i = optind; i < argc; i++) {
        if (process_file(fl, argv[i], &re) != 0) {
          fl.error_flag = 1;  // Установка флага ошибки
        }
      }
    }

    regfree(&re);
  }

  return fl.error_flag ? 1 : 0;  // Возвращаем код ошибки или успеха
}

Flags parse_flags(int argc, char *argv[], Flags *fl) {
  int rez, option_index = -1;
  opterr = 0;

  while ((rez = getopt_long(argc, argv, "e:ivclnhsf:o", NULL, &option_index)) !=
         -1) {
    switch (rez) {
      case 'e':
        fl->e = 1;
        add_pattern(fl, optarg);
        break;

      case 'i':
        fl->i = REG_ICASE;
        break;

      case 'v':
        fl->v = 1;
        break;

      case 'c':
        fl->c = 1;
        break;

      case 'l':
        fl->l = 1;
        break;

      case 'n':
        fl->n = 1;
        break;

      case 'h':
        fl->h = 1;
        break;

      case 's':
        fl->s = 1;
        break;

      case 'f':
        fl->f = 1;
        add_patt_from_file(fl, optarg);
        break;

      case 'o':
        fl->o = 1;
        break;

      case '?':  // Обработка недопустимого флага
        fprintf(stderr, "%s", "Error: unknown option\n");
        fl->error_flag = 1;  // Установка флага ошибки
                             //   return *fl;
    }
  }

  if (fl->len_pattern == 0 && optind < argc) {
    add_pattern(fl, argv[optind]);
    optind++;
  }

  if (argc - optind == 1) {
    fl->h = 1;  // Если остался только один параметр
  }

  return *fl;
}

void add_pattern(Flags *fl, char *pattern) {
  int len_pat = strlen(pattern);

  if (fl->len_pattern == 0) {
    fl->pattern = malloc(1024 * sizeof(char));
    fl->memory_pattern = 1024;
  }

  if (fl->memory_pattern <
      fl->len_pattern + len_pat + 2) {  // +2 для разделителя и конца строки
    fl->pattern = realloc(fl->pattern, fl->memory_pattern * 2);
    fl->memory_pattern *= 2;  // Увеличиваем память
  }

  if (fl->len_pattern != 0) {
    strcat(fl->pattern + fl->len_pattern, "|");
    fl->len_pattern++;
  }

  fl->len_pattern += sprintf(fl->pattern + fl->len_pattern, "%s", pattern);
}

void add_patt_from_file(Flags *fl, char *file) {
  FILE *fp = fopen(file, "r");
  if (fp == NULL) {
    if (!fl->s) {
      fprintf(stderr, "%s", "Error: no such file\n");
    }
    fl->error_flag = 1;  // Установка флага ошибки
    return;              // Возвращаемся, а не выходим
  }

  char line[BUFFER_SIZE];
  while (fgets(line, sizeof(line), fp)) {
    size_t len = strlen(line);
    if (len > 0 && line[len - 1] == '\n') {
      line[len - 1] = '\0';
    }

    // Пропуск пустых строк
    if (strlen(line) == 0) {
      continue;  // Переход к следующей итерации цикла
    }

    add_pattern(fl, line);
  }
  fclose(fp);
}

int process_file(Flags fl, char *file, regex_t *reg) {
  FILE *fp = fopen(file, "r");
  if (fp == NULL) {
    if (!fl.s) {
      fprintf(stderr, "%s", "Error: no such file or directory\n");
    }
    fl.error_flag = 1;  // Установка флага ошибки
  }

  int line_cnt = 1, match_cnt = 0;
  char line[BUFFER_SIZE];

  while (fgets(line, sizeof(line), fp)) {
    int result = regexec(reg, line, 0, NULL, 0);
    if ((result == 0 && !fl.v) || (fl.v && result != 0)) {
      if (!fl.c && !fl.l) {
        if (fl.o && !fl.v) {
          print_match(fl, reg, line, line_cnt, file);
        } else {
          match_flags(fl, line_cnt, file);
          output_line(line);
        }
      }
      match_cnt++;
    }
    line_cnt++;
  }

  finalize_output(fl, match_cnt, file);
  fclose(fp);
  return 0;  // Успешное завершение
}

void finalize_output(Flags fl, int match_cnt, char *file) {
  if (fl.c) {
    if (!fl.h) printf("%s:", file);
    if (fl.l && match_cnt > 0) {
      match_cnt = 1;
    }
    printf("%d\n", match_cnt);
  }
  if (fl.l && match_cnt > 0) {
    printf("%s\n", file);
  }
}

void print_match(Flags fl, regex_t *reg, char *line, int line_cnt, char *file) {
  regmatch_t match;
  int shift = 0;

  while (1) {
    int result = regexec(reg, line + shift, 1, &match, 0);
    if (result != 0) {
      break;
    }
    match_flags(fl, line_cnt, file);
    for (int i = match.rm_so; i < match.rm_eo; i++) {
      putchar(line[i + shift]);
    }
    putchar('\n');
    shift += match.rm_eo;
  }
}

void match_flags(Flags fl, int line_cnt, char *file) {
  if (!fl.h) {
    printf("%s:", file);
  }
  if (fl.n) {
    printf("%d:", line_cnt);
  }
}

void output_line(char *line) { printf("%s", line); }
