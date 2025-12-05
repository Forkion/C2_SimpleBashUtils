#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CONST 5000
#define MAX_SIZE_STR 256

struct flags {
  int b;
  int n;
  int e;
  int s;
  int t;
  int v;
} all_flags;

void open_file_and_output(char buff_names_files[][MAX_SIZE_STR],
                          int count_max_quantity_files, struct flags all_flags);
void output_print(char buf[][CONST], int pointer_on_str,
                  struct flags all_flags);
void parser(int argc, char **argv, char (*buff_names_files)[MAX_SIZE_STR],
            int *count_max_quantity_files, int *count_end_reading_flags);

int main(int argc, char **argv) {
  char buff_names_files[argc][MAX_SIZE_STR];
  memset(buff_names_files, 0, sizeof(buff_names_files));
  int count_max_quantity_files = 0;
  int count_end_reading_flags = 0;

  parser(argc, argv, buff_names_files, &count_max_quantity_files,
         &count_end_reading_flags);
  if (all_flags.b == 1) all_flags.n = 0;

  open_file_and_output(buff_names_files, count_max_quantity_files, all_flags);
  return 0;
}

void parser(int argc, char **argv, char (*buff_names_files)[MAX_SIZE_STR],
            int *count_max_quantity_files, int *count_end_reading_flags) {
  for (int i = 1; i < argc; ++i) {
    for (int j = 0; argv[i][j] != '\0'; ++j) {
      if (argv[i][0] == '-' && *count_end_reading_flags == 0) {
        if (strcmp(argv[i], "--number-nonblank") == 0) {
          all_flags.b = 1;
          break;
        }  //  +    "-b"
        if (strcmp(argv[i], "--number") == 0) {
          all_flags.n = 1;
          break;
        }  //  +    "-n"
        if (strcmp(argv[i], "--squeeze-blank") == 0) {
          all_flags.s = 1;
          break;
        }  //  +    "-s"
        j++;
        if (argv[i][j] == 'e') {
          all_flags.e = 1;
          all_flags.v = 1;
          continue;
        }  //  +    Отображает символ конца строки как '$' + "-v"
        else if (argv[i][j] == 't') {
          all_flags.t = 1;
          all_flags.v = 1;
          continue;
        }  //  +    Удаляет повторяющиеся пустые строки + "-v"
        else if (argv[i][j] == 'b') {
          all_flags.b = 1;
          continue;
        }  //  +    Нумерует непустые строки, важнее "-n"
        else if (argv[i][j] == 'n') {
          all_flags.n = 1;
          continue;
        }  //  +    Нумерует все строки
        else if (argv[i][j] == 'E') {
          all_flags.e = 1;
          continue;
        }  //  +    Отображает символ конца строки как '$'
        else if (argv[i][j] == 's') {
          all_flags.s = 1;
          continue;
        }  //  +    Удаляет повторяющиеся пустые строки
        else if (argv[i][j] == 'T') {
          all_flags.t = 1;
          continue;
        }  //  +    Отображает символ табуляции как "^I"
        else if (argv[i][j] == 'v') {
          all_flags.v = 1;
          continue;
        }  //  +    Отображает непичатаемые символы
        else {
          fprintf(stderr, "cat: invalid option -- '%c'\n", argv[i][j]);
          fprintf(stderr, "Try 'cat --help' for more information.\n");
          exit(1);
        }
      }
      if (argv[i][j] == '-' && *count_end_reading_flags == 1) {
        fprintf(stderr, "cat: %s: No such file or directory\n", argv[i]);
        break;
      } else if (i >= 1 && argv[i][j + 1] != '-') {
        strcpy(buff_names_files[(*count_max_quantity_files)++], argv[i]);
        *count_end_reading_flags = 1;
        break;
      }
    }
  }
}

void open_file_and_output(char buff_names_files[][MAX_SIZE_STR],
                          int count_max_quantity_files,
                          struct flags all_flags) {
  FILE *file = NULL;
  char buf[200][CONST] = {{'0'}};
  char input = 0;
  int number_input_file = 0;
  char output_sign;
  int pointer_on_str = 0;

  while (number_input_file < count_max_quantity_files) {
    if ((file = fopen(buff_names_files[number_input_file++], "r")) == NULL) {
      fprintf(stderr, "cat: %s: No such file or directory\n",
              buff_names_files[number_input_file - 1]);
      continue;
    }

    for (int sign = 0; sign < CONST; ++sign) {
      output_sign = fgetc(file);
      if (output_sign == EOF) break;
      buf[pointer_on_str][sign] = output_sign;
    }

    ++pointer_on_str;
    fclose(file);

    if (input == '\n') break;
  }

  if (all_flags.b == 1) all_flags.n = 0;
  output_print(buf, pointer_on_str, all_flags);
}

void output_print(char buf[][CONST], int pointer_on_str,
                  struct flags all_flags) {
  int number_str = 1;
  for (int str = 0; str < pointer_on_str; ++str) {
    for (int sign = 0; buf[str][sign] != '\0'; ++sign) {
      if (all_flags.v == 1 && ((buf[str][sign] >= 0 && buf[str][sign] < 32 &&
                                buf[str][sign] != 10 && buf[str][sign] != 9) ||
                               buf[str][sign] == 127)) {
        if (buf[str][sign] != 127) {
          printf("^%c", buf[str][sign] + 64);
        } else {
          printf("^?");
        }
        continue;
      }
      if (all_flags.s == 1 && buf[str][sign - 1] == '\n' &&
          buf[str][sign] == '\n' && buf[str][sign + 1] == '\n')
        continue;
      if (all_flags.e == 1 && buf[str][sign] == '\n') printf("$");
      if (all_flags.b == 1 &&
          ((sign == 0 && buf[str][sign] != '\n') ||
           (buf[str][sign - 1] == '\n' && buf[str][sign] != '\n')))
        printf("%6d\t", number_str++);
      if (all_flags.n == 1) {
        if (sign == 0) printf("%6d\t", number_str++);
        if (sign != 0 && buf[str][sign] == '\n' && buf[str][sign + 1] != '\0') {
          printf("\n%6d\t", number_str++);
          continue;
        }
      }
      if (all_flags.t == 1 && buf[str][sign] == '\t') {
        printf("^I");
        continue;
      }
      printf("%c", buf[str][sign]);
    }
  }
}
