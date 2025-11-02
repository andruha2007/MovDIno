// executor.h

#ifndef EXECUTOR_H
#define EXECUTOR_H

#include "../data/field.h"

int execute_size(Field **field, int width, int height, int line_number);

int execute_start(Field *field, int x, int y, int line_number);

int execute_move(Field *field, Direction direction, int line_number);

int execute_paint(Field *field, char color, int line_number);

int execute_jump(Field *field, Direction direction, int distance, int line_number);

int execute_dig(Field *field, Direction direction, int line_number);

int execute_mound(Field *field, Direction direction, int line_number);

int execute_grow(Field *field, Direction direction, int line_number);

int execute_cut(Field *field, Direction direction, int line_number);

int execute_make(Field *field, Direction direction, int line_number);

int execute_push(Field *field, Direction direction, int line_number);



#endif