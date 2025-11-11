// executor.h

#ifndef EXECUTOR_H
#define EXECUTOR_H

#include "../data/field.h"
#include "interpreter.h"

int execute_size(Field **field, Field **history_field, int width, int height, int line_number);

int execute_start(Field *field, Field *history_field, int x, int y, int line_number);

int execute_move(Field *field, Field *history_field, Direction direction, int line_number);

int execute_paint(Field *field, Field *history_field, char color, int line_number);

int execute_jump(Field *field, Field *history_field, Direction direction, int distance, int line_number);

int execute_dig(Field *field, Field *history_field, Direction direction, int line_number);

int execute_mound(Field *field, Field *history_field,  Direction direction, int line_number);

int execute_grow(Field *field, Field *history_field, Direction direction, int line_number);

int execute_cut(Field *field, Field *history_fiel0d, Direction direction, int line_number);

int execute_make(Field *field, Field *history_field, Direction direction, int line_number);

int execute_push(Field *field, Field *history_field, Direction direction, int line_number);

int execute_load(Field **field, Field **history_field, const char *filename, int line_number);

int execute_undo(Field *field, Field *history_field, int line_number);

int execute_if(Interpreter *interpreter, Field *field, Command *cmd);

#endif