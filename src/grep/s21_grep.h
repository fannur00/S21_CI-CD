#ifndef GREP_H
#define GREP_H

#define _GNU_SOURCE

#include <ctype.h>
#include <getopt.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  int regexp;
  int file;
  int ignore_case;
  int invert_match;
  int count;
  int files_with_matches;
  int only_matching;
  int no_filename;
  int line_number;
  int no_messages;
  char pattern[1024];
  regex_t regex;
  int len_pattern;
  regmatch_t pmatch;

} Flags;

void read_patterns_from_file(char *file_name, Flags *flags);
void print_file(char *name, Flags flags);
Flags argument_parser(int argc, char *argv[], Flags flags);
void search_file(FILE *file, Flags flags, char *name);
void add_pattern(Flags *flags, char *pattern);
void only_matching_func(int *match, Flags *flags, char line[],
                        int *line_number);

#endif