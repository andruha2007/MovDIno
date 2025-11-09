// specific_parser.H

#ifndef SPECIFIC_PARSER_H
#define SPECIFIC_PARSER_H

#include "parser.h"
#include "../utils/errors.h"



Command parse_size_command(int line_number);

Command parse_start_command(int line_number);

Command parse_move_command(int line_number);

Command parse_paint_command(int line_number);

Command parse_dig_command(int line_number);

Command parse_mound_command(int line_number);

Command parse_jump_command(int line_number);

Command parse_grow_command(int line_number);

Command parse_cut_command(int line_number);

Command parse_size_command(int line_number);

Command parse_make_command(int line_number);

Command parse_push_command(int line_number);

Command parse_exec_command(int line_number);

Command parse_load_command(int line_number);

Command parse_if_command(int line_number);

#endif