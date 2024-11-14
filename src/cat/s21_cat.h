#ifndef GREP_H
#define GREP_H

#include <getopt.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define _GNU_SOURCE

typedef struct {
  int show_ends;
  int number_non_blank;
  int number;
  int show_nonprinting;
  int show_tabs;
  int squeeze_blank;
} Flags;

void print_file(char *name, Flags flags);
Flags argument_parser(int argc, char *argv[], Flags flags);

void print_symble(FILE *file, char c, int start_of_line, Flags flags);

void squeeze_blank(FILE *file, Flags flags, char c, char prev_c);

void print_number(Flags flags, char c, int start_of_line);

void show_ends(Flags flags, char c, int start_of_line);

char convert_char(char c);
bool should_convert_char(char c, Flags flags);

#endif
