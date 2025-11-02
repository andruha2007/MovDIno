// errors.c

#include "errors.h"
#include <stdio.h>
#include <stdlib.h>
#include "../utils/common.h"

void handle_warning(const char *message, int line_number){
    if (line_number > 0)
        fprintf(stderr, COLOR_RED "WARNING: at line %d: %s\n" COLOR_RESET, line_number, message);
    else
        fprintf(stderr, COLOR_RED "WARNING: %s\n" COLOR_RESET, message);
}

void handle_error(const char *message, int line_number){
    if (line_number > 0)
        fprintf(stderr, COLOR_RED "ERROR: at line %d: %s\n" COLOR_RESET, line_number, message);
    else
        fprintf(stderr, COLOR_RED "ERROR: %s\n" COLOR_RESET, message);
    exit(EXIT_FAILURE);
}
