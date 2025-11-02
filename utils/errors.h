// errors.h

#ifndef ERRORS_H
#define ERRORS_H

typedef enum {
    ERROR_LEVEL_WARNING, // Предупреждение, программа продолжает работу
    ERROR_LEVEL_ERROR, // Критическая ошибка, прекращение выполнения
} ErrorLevel;


void handle_warning(const char *message, int line_number);

void handle_error(const char *message, int line_number);

#endif