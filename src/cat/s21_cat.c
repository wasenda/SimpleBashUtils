#include "s21_cat.h"

int main(int argc, char *argv[]) {
  FILE *fp;
  Flags fl = {0};
  fl.error = 0;

  if (argc > 1) {
    if (parse_flags(argc, argv, &fl) != 1 && fl.error != 1) {
      int next_arg = optind;
      while (next_arg < argc) {
        fp = fopen(argv[next_arg], "r");
        if (fp == NULL) {
          fprintf(stderr, "%s No such file or directory\n", argv[next_arg]);
          fl.error = 1;
        } else {
          print_file(fp, &fl);
          fclose(fp);
        }
        next_arg++;
      }
    }
  }

  return 0;
}

int parse_flags(int argc, char *argv[], Flags *fl) {
  int rez, option_index = -1;
  opterr = 0;

  while ((rez = getopt_long(argc, argv, short_option, long_option,
                            &option_index)) != -1) {
    switch (rez) {
      case 'b':
        fl->b = 1;
        break;

      case 'e':
        fl->e = 1;
        fl->v = 1;
        break;

      case 'E':
        fl->e = 1;
        break;

      case 'n':
        fl->n = fl->b ? 0 : 1;
        break;

      case 's':
        fl->s = 1;
        break;

      case 't':
        fl->t = 1;
        fl->v = 1;
        break;

      case 'T':
        fl->t = 1;
        break;

      case 'v':
        fl->v = 1;
        break;

      default:
        fprintf(stderr, "%s\n", "Error, wrong option");
        fl->error = 1;
    }
  }
  return 0;
}

FILE *print_file(FILE *fp, Flags *fl) {
  int current_sym, cnt_line = 0, cnt_empty = 0;
  int last_sym = '\n';

  if (fl->b) {
    fl->n = 0;
  }

  while ((current_sym = fgetc(fp)) != EOF) {
    if (fl->s && current_sym == '\n' && last_sym == '\n') {
      cnt_empty++;
      if (cnt_empty > 1) {
        continue;
      }
    } else {
      cnt_empty = 0;
    }

    if (last_sym == '\n' && ((fl->b && current_sym != '\n') || fl->n)) {
      printf("%6d\t", ++cnt_line);
    }

    if (fl->e) {
      if (fl->b && current_sym == '\n' && last_sym == '\n') {
        printf("      \t");
      }
      if (current_sym == '\n') {
        printf("$");
      }
    }

    if (fl->v) {
      if (current_sym > 127 && current_sym < 160) {
        printf("M-^");
      }
      if (current_sym == 127 ||
          (current_sym < 32 && current_sym != '\n' && current_sym != '\t')) {
        printf("^");
      }
      if ((current_sym < 32 || (current_sym > 126 && current_sym < 160)) &&
          current_sym != '\n' && current_sym != '\t') {
        if (current_sym > 126) {
          current_sym = current_sym - 128 + 64;
        } else {
          current_sym += 64;
        }
      }
    }

    if (fl->t && current_sym == '\t') {
      printf("^I");
      continue;
      // current_sym = 'I';
    }

    printf("%c", current_sym);
    last_sym = current_sym;
  }

  return fp;
}
