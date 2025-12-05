#ifndef PARSER_H
#define PARSER_H

#include <regex.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "getopt.h"

#define CONST 1000

typedef struct {
  bool e;
  bool i;
  bool v;
  bool c;
  bool l;
  bool n;
  bool h;
  bool s;
  bool f;
  bool o;
} flags;

typedef struct {
  char str[256][256];
  int size;
} reg_str;

int parser(int argc, char **argv, char *option_short, flags *all_flags,
           char (*buff_template)[256], int *count_template,
           char (*buff_template_files)[256], int *count_template_files);

void flag_preorety(flags *all_flags);

void parser_names_files(char **argv, int argc, char (*buff_template)[256],
                        int *count_template, char (*buff_template_files)[256],
                        int *count_template_files,
                        char (*buff_names_files)[256],
                        int *count_max_quantity_files);

void cpy_template_out_file(char (*buff_template)[256], int *count_template,
                           char (*buff_template_files)[256],
                           int *count_template_files);

void processing_file(FILE *file, char *file_name, flags *all_flags,
                     char (*buff_template)[256], int *count_template,
                     int *count_max_quantity_files);

void output_some_flags(FILE *file, char *str_from_file, flags *all_flags,
                       int str_number, int *count_max_quantity_files,
                       char *file_name, int *output_count, int *count_template,
                       regmatch_t *coincedence, char *result,
                       char (*buff_template)[256], reg_str *res);

int find_template(char *str_from_file, char *buff_template, flags *all_flags,
                  regmatch_t *coincedence, reg_str *res);

void flag_o(char *str_from_file, flags *all_flags, regmatch_t *coincedence,
            int str_number, int *count_max_quantity_files, char *file_name);

#endif
