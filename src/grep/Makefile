CC = gcc 
FLAGS = -Wall -Werror -Wextra -std=c11


s21_grep: s21_grep.c s21_grep.h
	$(CC) $(FLAGS) s21_grep.c -o s21_grep

cppcheck: s21_grep
	cppcheck --enable=all --suppress=missingIncludeSystem . s21_grep.c s21_grep.h

clean: 
	rm -f s21_grep	

test: s21_grep
	bash grep_test_1.sh

# test2: s21_grep
# 	bash grep_test_2.sh