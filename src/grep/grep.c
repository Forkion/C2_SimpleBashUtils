#define _GNU_SOURCE

#include "parser.h"

int main(int argc, char **argv) {
  FILE *file = NULL;
  flags all_flags = {0};
  char *option_short = "f:e:ivclnhso";
  int error = false;

  char buff_template[argc * 10][256];
  memset(buff_template, 0, sizeof(buff_template));
  int count_template = false;

  char buff_template_files[argc][256];
  memset(buff_template_files, 0, sizeof(buff_template_files));
  int count_template_files = false;

  char buff_names_files[argc][256];
  memset(buff_names_files, 0, sizeof(buff_names_files));
  int count_max_quantity_files = false;

  error = parser(argc, argv, option_short, &all_flags, buff_template,
                 &count_template, buff_template_files, &count_template_files);
  if (error == 2) exit(1);
  if (error) {
    fprintf(stderr, "grep: Invalid argument\n");
    exit(1);
  }

  parser_names_files(argv, argc, buff_template, &count_template,
                     buff_template_files, &count_template_files,
                     buff_names_files, &count_max_quantity_files);

  if (count_template_files > 0)
    cpy_template_out_file(buff_template, &count_template, buff_template_files,
                          &count_template_files);

  if (count_template == 0) {
    fprintf(stderr, "the template was not found\n");
    exit(1);
  }
  for (int i = 0; i < count_max_quantity_files; ++i) {
    if ((file = fopen(buff_names_files[i], "r")) == NULL) {
      if (all_flags.s == 0)
        fprintf(stderr, "grep: %s: No such file or directory\n",
                buff_names_files[i]);
      continue;
    }
    processing_file(file, buff_names_files[i], &all_flags, buff_template,
                    &count_template, &count_max_quantity_files);
    fclose(file);
  }
  return 0;
}

void parser_names_files(char **argv, int argc, char (*buff_template)[256],
                        int *count_template, char (*buff_template_files)[256],
                        int *count_template_files,
                        char (*buff_names_files)[256],
                        int *count_max_quantity_files) {
  int flag_coincidences2 = false;
  for (int i = 1; i < argc; ++i) {
    int flag_coincidences = false;

    for (int j = 0; j < *count_template; ++j) {
      if (argv[i][0] != '-' && flag_coincidences == 0 &&
          strcmp(argv[i], buff_template[j]) == 0) {
        flag_coincidences = 1;
        break;
      }
    }
    if (flag_coincidences == 0) {
      for (int l = 0; l < *count_template_files; ++l) {
        if (argv[i][0] != '-' &&
            (flag_coincidences == 0 || flag_coincidences2 == 0) &&
            strcmp(argv[i], buff_template_files[l]) == 0) {
          if (flag_coincidences) {
            flag_coincidences2 = 2;
            break;
          }
          flag_coincidences = 1;
          continue;
        }
      }
    }
    if (argv[i][0] != '-' &&
        (flag_coincidences == 0 || flag_coincidences2 == 2)) {
      strcpy(buff_names_files[(*count_max_quantity_files)++], argv[i]);
    }
  }
}

void cpy_template_out_file(char (*buff_template)[256], int *count_template,
                           char (*buff_template_files)[256],
                           int *count_template_files) {
  FILE *file = NULL;
  size_t length = false;
  char *str_from_file = NULL;
  for (int i = 0; i < *count_template_files; ++i) {
    file = fopen(buff_template_files[i], "r");
    while ((getline(&str_from_file, &length, file)) != EOF) {
      strcpy(buff_template[*count_template], str_from_file);
      (*count_template)++;
    }
    fclose(file);
  }
  for (int i = 0; i < *count_template; ++i)
    for (int j = 0; buff_template[i][j] != '\0'; ++j)
      if (buff_template[i][j] == '\n') {
        buff_template[i][j] = '\0';
        break;
      }
  free(str_from_file);
}

void processing_file(FILE *file, char *file_name, flags *all_flags,
                     char (*buff_template)[256], int *count_template,
                     int *count_max_quantity_files) {
  size_t length = 0;
  char *str_from_file = NULL;
  int count_str_template = 0, str_number = 0;
  reg_str res;
  res.size = 0;
  for (int i = 0; i < 256; i++) {
    memset(res.str, 0, sizeof(res.str[0]) * 256);
  }

  while ((getline(&str_from_file, &length, file)) !=
         EOF) {  // считываю строку из файла
    str_number++;
    res.size = 0;

    if (str_from_file == NULL && str_from_file[0] != '\n') {
      str_number++;
      continue;
    }

    regmatch_t coincedence[strlen(str_from_file)];

    for (int i = 0; i < *count_template; ++i) {
      find_template(str_from_file, buff_template[i], all_flags, coincedence,
                    &res);  // записываю найденные по шаблонам совпадения
    }
    if (all_flags->v) res.size = !res.size;

    // Обработка вывода с флагами
    if (res.size) {
      if (all_flags->l) {  // -l
        printf("%s\n", file_name);
        break;
      } else if (all_flags->c) {  // счёт строк для -c
        count_str_template++;
      } else if (all_flags->o) {  // -o НАЧАЛО
        if (!all_flags->v) {      // -ov
          for (int j = 0; j < res.size; j++) {
            if (all_flags->n) {  // -on
              if (all_flags->h ||
                  *count_max_quantity_files == 1) {  // -hon либо 1 файл
                printf("%d:%s\n", str_number, res.str[j]);
              } else {
                printf("%s:%d:%s\n", file_name, str_number, res.str[j]);
              }
            } else if (all_flags->h || *count_max_quantity_files == 1) {  // -ho
              printf("%s\n", res.str[j]);
            } else {
              printf("%s:%s\n", file_name, res.str[j]);
            }
          }
        }  // -o КОНЕЦ
      } else {
        if (all_flags->n) {  // -n
          if (all_flags->h ||
              *count_max_quantity_files == 1) {  // -hn либо 1 файл
            printf("%d:%s", str_number, str_from_file);
          } else {
            printf("%s:%d:%s", file_name, str_number, str_from_file);
          }
        } else {  // -h
          if (all_flags->h ||
              *count_max_quantity_files == 1) {  // -hn либо 1 файл
            printf("%s", str_from_file);
          } else {
            printf("%s:%s", file_name, str_from_file);
          }
        }
        if (str_from_file[strlen(str_from_file) - 1] != '\n') printf("\n");
      }
    }
  }

  if (all_flags->c) {                                      // -c
    if (all_flags->h || *count_max_quantity_files == 1) {  // -hc либо 1 файл
      printf("%d\n", count_str_template);
    } else {
      printf("%s:%d\n", file_name, count_str_template);
    }
  }

  free(str_from_file);
}

int find_template(char *str_from_file, char *buff_template, flags *all_flags,
                  regmatch_t *coincedence, reg_str *res) {
  regex_t template_regex;
  int reg_flags = REG_EXTENDED;
  if (all_flags->i) reg_flags |= REG_ICASE;

  if (str_from_file[0] != '\0') {
    if (regcomp(&template_regex, buff_template, reg_flags)) {
      exit(1);
    }
    char *search_ptr = str_from_file;
    while (res->size < 256 &&
           !regexec(&template_regex, search_ptr, 1, coincedence, 0)) {
      int start_index = (search_ptr - str_from_file) + coincedence->rm_so;
      int end_index = (search_ptr - str_from_file) + coincedence->rm_eo;

      strncpy(res->str[res->size], str_from_file + start_index,
              end_index - start_index);

      res->str[res->size][end_index - start_index + 1] = '\0';
      search_ptr = str_from_file + end_index;
      res->size++;

      if (coincedence->rm_so == coincedence->rm_eo) break;
    }

    regfree(&template_regex);
  }
  return 0;
}
