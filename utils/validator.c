// validator.c

#include "validator.h"
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <limits.h>

int is_strict_integer(const char *str){
    if (str == NULL || *str == '\0')
        return -1;

    for (int i = 0; str[i] != '\0'; ++i) {
        if (!isdigit((unsigned char) str[i]))
            return -1;
    }
    return 1;
}


int parse_strict_positive_integer(const char *str, int *result){
    if (is_strict_integer(str) == -1)
        return -1;

    char *endptr;
    errno = 0;
    long value = strtol(str, &endptr, 10);

    if (errno == ERANGE) return 0;
    if (*endptr != '\0') return 0;
    if (value < INT_MIN || value > INT_MAX) return 0;

    *result = atoi(str);

    if (result < 0)
        return -1;
        
    return 1;
}

int is_in_range(int value, int min, int max){
    return (value >= min && value <= max);
}

int are_valid_coordinates(int x, int y, int max_x, int max_y){
    return (x >= 0 && x < max_x && y >= 0 && y < max_y);
}

int is_valid_color_char(char c){
    return ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'));
}

int is_valid_ceil_symbol(char c){
    return c == '_' || c == '#' || c == '%' || c == '%' ||
           c == '^' || c == '&' || c == '@'
           || (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

int has_left_spaces(const char* line) {
    if (!line || *line == '\0') return 0;
    return (isspace(*line));
}

int is_whitespace_only(const char* line){
    if (!line) return 1;

    for(int i = 0; line[i] != '\0'; i++){
        if (!isspace((unsigned char) line[i]))
            return 0;
    }

    return 1;
}

int is_comment_line(const char* line){
    if (!line) return 0;

    const char *ptr = line;
    while (*ptr && isspace((unsigned char) *ptr)) ptr++;
    return (strncmp(ptr, "//", 2) == 0);
}


int is_valid_direction(const char* dir_str) {
    if (!dir_str) return 0;
    return (strcmp(dir_str, "UP") == 0 || strcmp(dir_str, "DOWN") == 0 ||
            strcmp(dir_str, "LEFT") == 0 || strcmp(dir_str, "RIGHT") == 0);
}

