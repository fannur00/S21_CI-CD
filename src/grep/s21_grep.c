#include "s21_grep.h"

int main(int argc, char **argv) {
  Flags flags = {0};

  flags = argument_parser(argc, argv, flags);

  for (int index = optind; index < argc; index++) {
    print_file(argv[index], flags);
  }

  return 0;
}

void print_file(char *name, Flags flags) {
  FILE *file = fopen(name, "r");
  if (file == NULL) {
    if (!flags.no_messages) perror(name);
    exit(1);
  }

  if (regcomp(&flags.regex, flags.pattern,
              REG_EXTENDED | (flags.ignore_case ? REG_ICASE : 0))) {
    fprintf(stderr, "Could not compile regex\n");
    exit(1);
  }
  search_file(file, flags, name);

  fclose(file);
}

Flags argument_parser(int argc, char *argv[], Flags flags) {
  int rez;
  while ((rez = getopt(argc, argv, "e:f:isvnholc")) != -1) {
    switch (rez) {
      case 'e':
        flags.regexp = 1;
        add_pattern(&flags, optarg);
        break;
      case 'f':
        flags.file = 1;
        read_patterns_from_file(optarg, &flags);
        break;
      case 'i':
        flags.ignore_case = 1;
        break;
      case 's':
        flags.no_messages = 1;
        break;
      case 'v':
        flags.invert_match = 1;
        break;
      case 'n':
        flags.line_number = 1;
        break;
      case 'h':
        flags.no_filename = 1;
        break;
      case 'o':
        flags.only_matching = 1;
        break;
      case 'l':
        flags.files_with_matches = 1;
        break;
      case 'c':
        flags.count = 1;
        break;
      case '?':
        printf("Error found !\n");
        break;
    }
  }
  if (flags.len_pattern == 0) {
    add_pattern(&flags, argv[optind++]);
  }

  if (argc - optind == 1) {
    flags.no_filename = 1;
  }

  return flags;
}

void search_file(FILE *file, Flags flags, char *name) {
  char line[1024];
  int line_number = 1, match_count = 0, match, count;
  while (fgets(line, sizeof(line), file) != NULL) {
    int newline_pos = strcspn(line, "\n");
    if (newline_pos == (int)strlen(line)) {
      line[newline_pos] = '\n';
      line[newline_pos + 1] = '\0';
    }
    match = regexec(&flags.regex, line, 1, &flags.pmatch, 0);
    if ((match == 0 && !flags.invert_match) ||
        (match != 0 && flags.invert_match)) {
      match_count++;
      if (flags.invert_match && flags.files_with_matches)
        flags.invert_match = 0;
      if ((!flags.count && !flags.files_with_matches && !flags.only_matching) ||
          (flags.only_matching && flags.invert_match)) {
        if (flags.line_number && flags.no_filename) printf("%d:", line_number);
        if (!flags.no_filename && !flags.count) {
          printf("%s:", name);
          if (flags.line_number) printf("%d:", line_number);
        }
        if (!flags.count && !flags.files_with_matches) printf("%s", line);
      }
      if (flags.count && flags.files_with_matches) count = 1;
    }
    if (flags.only_matching && !flags.files_with_matches &&
        !flags.invert_match && !flags.count) {
      if (!flags.no_filename && match == 0) {
        printf("%s:", name);
        if (flags.line_number) printf("%d:", line_number);
      }
      only_matching_func(&match, &flags, line, &line_number);
    }
    line_number++;
  }
  if (flags.count) {
    if (!flags.no_filename) printf("%s:", name);
    if (!flags.files_with_matches) {
      printf("%d\n", match_count);
    } else {
      printf("%d\n", count);
    }
  }
  if (flags.files_with_matches && match_count > 0) printf("%s\n", name);
  regfree(&flags.regex);
}

void read_patterns_from_file(char *file_name, Flags *flags) {
  FILE *file = fopen(file_name, "r");
  if (file == NULL) {
    perror(file_name);
    exit(1);
  }

  char pattern[1024];

  while (fgets(pattern, sizeof(pattern), file) != NULL) {
    pattern[strcspn(pattern, "\n")] = 0;
    add_pattern(flags, pattern);
  }

  fclose(file);
}

void add_pattern(Flags *flags, char *pattern) {
  if (flags->len_pattern != 0) {
    strcat(flags->pattern + flags->len_pattern, "|");
    flags->len_pattern++;
  }
  flags->len_pattern +=
      sprintf(flags->pattern + flags->len_pattern, "(%s)", pattern);
}

void only_matching_func(int *match, Flags *flags, char line[],
                        int *line_number) {
  regoff_t offset = 0;
  while (*match == 0) {
    *match =
        regexec(&flags->regex, line + offset, 1, &flags->pmatch, REG_NOTBOL);
    if (*match == 0) {
      if (flags->line_number && flags->no_filename) printf("%d:", *line_number);

      for (int i = flags->pmatch.rm_so + offset;
           i < flags->pmatch.rm_eo + offset; i++) {
        printf("%c", line[i]);
      }

      printf("\n");
      offset += (flags->pmatch.rm_eo + flags->pmatch.rm_so);
    }
  }
}