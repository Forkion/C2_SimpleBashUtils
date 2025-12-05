#include "parser.h"

int parser(int argc, char **argv, char *option_short, flags *all_flags,
           char (*buff_template)[256], int *count_template,
           char (*buff_template_files)[256], int *count_template_files) {
  int res = 0, error = 0, index;
  FILE *template_file = NULL;
  static struct option long_options[] = {{0, 0, 0, 0}};
  opterr = 0;
  while ((res = getopt_long(argc, argv, option_short, long_options, &index)) !=
         -1) {
    switch (res) {
      case 'e':
        all_flags->e = true;  //  +  Чтение несколькох шаблонов
        strcpy(buff_template[*count_template], optarg);
        (*count_template)++;
        break;
      case 'i':
        all_flags->i = true;  //  +  Игнорировать регистр при поиске совпадений
        break;
      case 'v':
        all_flags->v = true;  //  +  Вывод строк без совпадений
        break;
      case 'c':
        all_flags->c = true;  //  +  Вывести только сумму найденных строк
        break;
      case 'l':
        all_flags->l =
            true;  //  +  Вывод только файлов, где были найдены соответствия
        break;
      case 'n':
        all_flags->n = true;  //  +  Номер строки
        break;
      case 'h':
        all_flags->h = true;  //  +  Не выводить имя файла
        break;
      case 's':
        all_flags->s =
            true;  //  +  Не выводить ошибки отсутствия файла для поиска
        break;
      case 'f':
        all_flags->f = true;  //  +-  Чтение шаблонов из файла (DODELAT' если
                              //  шаблон перед именем файла)
        strcpy(buff_template_files[*count_template_files], optarg);
        (*count_template_files)++;
        break;
      case 'o':
        all_flags->o = true;  //  -  Чтение только находимого шаблона
        break;
      case '?':
        error = 1;  //  +  Вывод ошибки флага
        break;
      default:
        error = 1;  //  +  Вывод ошибки флага
        break;
    }
  }
  for (int i = 0; i < *count_template_files; ++i) {
    if ((template_file = fopen(buff_template_files[i], "r")) == NULL) {
      fprintf(stderr, "grep: %s: No such file or directory\n",
              buff_template_files[i]);
      error = 2;
      break;
    }
    fclose(template_file);
  }

  if (!all_flags->f && !all_flags->e) {
    for (int i = 1; i < argc; ++i) {
      if (argv[i][0] != '-') {
        strcpy(buff_template[*count_template], argv[i]);
        (*count_template)++;
        break;
      }
    }
  }

  flag_preorety(all_flags);
  return error;
}

void flag_preorety(flags *all_flags) {
  if (all_flags->l) {
    all_flags->c = false;
    all_flags->o = false;
    all_flags->n = false;
    return;
  }
  if (all_flags->c) {
    all_flags->o = false;
  }
}
