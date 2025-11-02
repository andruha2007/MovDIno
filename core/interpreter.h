// interpreter.h

#ifndef INTERPRETER_H
#define INTERPRETER_H


#include "../utils/common.h"
#include "../data/field.h"
#include "../parser/parser.h"


typedef struct{
    char input_file[MAX_FILENAME];
    char output_file[MAX_FILENAME];
    int should_display;
    int should_save;
    int display_interval;
} InterpreterConfig;

typedef struct{
    Field *field;
    Command *command;
    CommandList *command_list;
    InterpreterConfig config;
    int current_command;
    int execution_success;
} Interpreter;

typedef enum {
    EXECUTION_SUCCESS = 1,
    EXECUTION_WARNING = 0,
    EXECUTION_ERROR = -1,
} ExecutionResult;

// Функции интерпретатора

void sleep_ms(int milliseconds);
void clear_screen(void);


Interpreter *create_interpreter(void);
void free_interpreter(Interpreter *interpreter);
int execute_program(Interpreter *interpreter, const char *input_file, const char *output_file);
int execute_command(Interpreter *interpreter, Command *cmd, int line_number);

#endif