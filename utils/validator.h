// validator.h

#ifndef VALIDATOR_H
#define VALIDATOR_H

// Проверка целых чисел
int is_strict_integer(const char *str);
int parse_strict_positive_integer(const char *str, int *result);

// Проверка диапазона
int is_in_range(int value, int min, int max);

// Проверка координат
int are_valid_coordinates(int x, int y, int max_x, int max_y);

// Проверка символов
int is_valid_color_char(char c);
int is_valid_ceil_symbol(char c);

// Проверка строк и формата
int has_left_spaces(const char* line);
int is_whitespace_only(const char* line);
int is_comment_line(const char* line);

// Проверка направлений
int is_valid_direction(const char* dir_str);

#endif