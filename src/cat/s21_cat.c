#include "s21_cat.h"

int main(int argc, char **argv) {
  Flags flags = {0};

  flags = argument_parser(argc, argv, flags);

  for (int index = optind; index < argc; index++) {
    print_file(argv[index], flags);
  }
  return 0;
}

void squeeze_blank(FILE *file, Flags flags, char c, char prev_c) {
  if (flags.squeeze_blank && c == '\n' && prev_c == '\n') {
    c = fgetc(file);
    while (c == '\n') {
      c = fgetc(file);
    }
    ungetc(c, file);
  }
}

void print_number(Flags flags, char c, int start_of_line) {
  static int line_number = 1;

  if (flags.number_non_blank) {
    flags.number = 0;
  }

  if ((flags.number && start_of_line) ||
      (flags.number_non_blank && c != '\n' && start_of_line)) {
    printf("%6u\t", line_number++);
  }
}

void show_ends(Flags flags, char c, int start_of_line) {
  if (flags.show_ends && c == '\n') {
    if (flags.number_non_blank && start_of_line) {
      printf("%6c\t%c", ' ', '$');
    } else {
      putchar('$');
    }
  }
}

bool should_convert_char(char c, Flags flags) {
  return (c == '\t' && flags.show_tabs) ||
         (flags.show_nonprinting &&
          ((c <= 31 && c != '\n' && c != '\t') || (c == 127)));
}

char convert_char(char c) { return (c == 127) ? c - 64 : c + 64; }

void print_symble(FILE *file, char c, int start_of_line, Flags flags) {
  char prev_c = '\n';

  while (c != EOF) {
    squeeze_blank(file, flags, c, prev_c);
    print_number(flags, c, start_of_line);
    show_ends(flags, c, start_of_line);

    if (should_convert_char(c, flags)) {
      putchar('^');
      putchar(convert_char(c));
    } else {
      putchar(c);
    }

    if (c == '\n') {
      start_of_line = 1;
    } else {
      start_of_line = 0;
    }

    prev_c = c;
    c = fgetc(file);
  }
}

Flags argument_parser(int argc, char *argv[], Flags flags) {
  struct option long_options[] = {
      {"number-nonblank", no_argument, &flags.number_non_blank, 1},
      {"number", no_argument, &flags.number, 1},
      {"squeeze-blank", no_argument, &flags.squeeze_blank, 1},
      {0, 0, 0, 0}};

  int rez;
  while ((rez = getopt_long(argc, argv, "+benstET", long_options, NULL)) !=
         -1) {
    switch (rez) {
      case 'b':
        flags.number_non_blank = 1;
        break;
      case 'e':
        flags.show_ends = 1;
        flags.show_nonprinting = 1;
        break;
      case 'n':
        flags.number = 1;
        break;
      case 's':
        flags.squeeze_blank = 1;
        break;
      case 't':
        flags.show_nonprinting = 1;
        flags.show_tabs = 1;
        break;
      case 'E':
        flags.show_ends = 1;
        break;
      case 'T':
        flags.show_tabs = 1;
        break;
      case '?':
        printf("Error found !\n");
        break;
    }
  }
  return flags;
}

void print_file(char *name, Flags flags) {
  FILE *file = fopen(name, "r");
  if (file == NULL) {
    perror("Error opening file");
    exit(1);
  }

  char c = fgetc(file);
  int start_of_line = 1;

  print_symble(file, c, start_of_line, flags);

  fclose(file);
}