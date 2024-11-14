CC = gcc 

FLAGS = -Wall -Werror -Wextra -std=c11 

s21_cat: s21_cat.c s21_cat.h
	$(CC) $(FLAGS) s21_cat.c -o s21_cat


cppcheck: s21_cat
	cppcheck --enable=all --suppress=missingIncludeSystem . s21_cat.c s21_cat.h

clean: 
	rm -f s21_cat

test: s21_cat
	bash test_cat.sh