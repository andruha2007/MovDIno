// specific_parser.c

#include "../utils/common.h"
#include "specific_parser.h"
#include "../utils/errors.h"
#include <stdio.h>
#include <string.h>
#include "../utils/validator.h"

Command parse_size_command(int line_number){
    Command cmd = {0};
    cmd.type = CMD_SIZE;
    cmd.line_number = line_number;

    char *width_str = strtok(NULL, " ");
    char *height_str = strtok(NULL, " ");

    int width, height;
    if (parse_strict_positive_integer(width_str, &width) == -1){
        handle_error("Invalid width in SIZE command", line_number);
        return (Command){CMD_UNKNOWN};
    }

    if (parse_strict_positive_integer(height_str, &height) == -1){
        handle_error("Invalid height in SIZE command", line_number);
        return (Command){CMD_UNKNOWN};
    }

    if (!is_in_range(width, MIN_WIDTH, MAX_WIDTH) ||
    !is_in_range(height, MIN_HEIGHT, MAX_HEIGHT)){
        handle_error("Field size is out of range", line_number);
        return (Command){CMD_UNKNOWN};
    }

    cmd.parametrs.size.width = width;
    cmd.parametrs.size.height = height;

    return cmd;    


}

Command parse_start_command(int line_number){
    Command cmd = {0};
    cmd.type = CMD_START;
    cmd.line_number = line_number;
    char* x_str = strtok(NULL, " ");
    char* y_str = strtok(NULL, " ");

    int x, y; 
    if (parse_strict_positive_integer(x_str, &x) == -1){
        handle_error("Invalid X-coordinate in START command", line_number);
        return (Command){CMD_UNKNOWN};
    }

    if (parse_strict_positive_integer(y_str, &y) == -1){
        handle_error("Invalid Y-coordinate in START command", line_number);
        return (Command){CMD_UNKNOWN};
    }

    cmd.parametrs.position.x = x;
    cmd.parametrs.position.y = y;

    return cmd;
}

Command parse_move_command(int line_number){
    Command cmd = {0};
    cmd.type = CMD_MOVE;
    cmd.line_number = line_number;

    char *dir_str = strtok(NULL, " ");

    if (!is_valid_direction(dir_str)){
        handle_error("Invalid direction in MOVE command", line_number);
        return (Command){CMD_UNKNOWN};
    }

    cmd.parametrs.direction = parse_direction(dir_str);
    return cmd;
}

Command parse_paint_command(int line_number){
    Command cmd = {0};
    cmd.type = CMD_PAINT;
    cmd.line_number = line_number;
    char *color_str = strtok(NULL, " ");
    
    if (!color_str || strlen(color_str) != 1){
        handle_error("Invalid color in PAINT command", line_number);
        return (Command){CMD_UNKNOWN};
    }

    if (!is_valid_color_char(color_str[0])){
        handle_error("Color must be lowercase (uppercase) ltter a-z (A-Z)", line_number);
        return (Command){CMD_UNKNOWN};
    }

    cmd.parametrs.color_char = color_str[0];

    return cmd;
}

Command parse_jump_command(int line_number){
    Command cmd = {0};
    cmd.type = CMD_JUMP;
    cmd.line_number = line_number;
    char *dir_str = strtok(NULL, " ");
    char *dist_str = strtok(NULL, " ");
    
    int distance;
    if (!is_valid_direction(dir_str)){
        handle_error("Invalid direction in JUMP command", line_number);
        return (Command){CMD_UNKNOWN};
    }

    if (parse_strict_positive_integer(dist_str, &distance) == -1) {
        handle_error("Invalid distance in JUMP command", line_number);
        return (Command){CMD_UNKNOWN};
    }
    
    cmd.parametrs.direction = parse_direction(dir_str);
    cmd.jump_distance = distance;

    return cmd;
}

Command parse_dig_command(int line_number){
    Command cmd = {0};
    cmd.type = CMD_DIG;
    cmd.line_number = line_number;
    char *dir_str = strtok(NULL, " ");

    if (!is_valid_direction(dir_str)){
        handle_error("Invalid direction in DIG command", line_number);
        return (Command){CMD_UNKNOWN};
    }

    cmd.parametrs.direction = parse_direction(dir_str);

    return cmd;
}

Command parse_mound_command(int line_number){
    Command cmd = {0};
    cmd.type = CMD_MOUND;
    cmd.line_number = line_number;
    char *dir_str = strtok(NULL, " ");

    if (!is_valid_direction(dir_str)){
        handle_error("Invalid direction in MOUND command", line_number);
        return (Command){CMD_UNKNOWN};
    }

    cmd.parametrs.direction = parse_direction(dir_str);

    return cmd;
}

Command parse_grow_command(int line_number){
    Command cmd = {0};
    cmd.type = CMD_GROW;
    cmd.line_number = line_number;
    char *dir_str = strtok(NULL, " ");

    if (!is_valid_direction(dir_str)){
        handle_error("Invalid direction in MOUND command", line_number);
        return (Command){CMD_UNKNOWN};
    }

    cmd.parametrs.direction = parse_direction(dir_str);

    return cmd;
}

Command parse_cut_command(int line_number){
    Command cmd = {0};
    cmd.type = CMD_CUT;
    cmd.line_number = line_number;
    char *dir_str = strtok(NULL, " ");

    if (!is_valid_direction(dir_str)){
        handle_error("Invalid direction in CUT command", line_number);
        return (Command){CMD_UNKNOWN};
    }

    cmd.parametrs.direction = parse_direction(dir_str);

    return cmd;
}

Command parse_make_command(int line_number){
    Command cmd = {0};
    cmd.type = CMD_MAKE;
    cmd.line_number = line_number;
    char *dir_str = strtok(NULL, " ");

    if (!is_valid_direction(dir_str)){
        handle_error("Invalid direction in MAKE command", line_number);
        return (Command){CMD_UNKNOWN};
    }

    cmd.parametrs.direction = parse_direction(dir_str);

    return cmd;
}

Command parse_push_command(int line_number){
    Command cmd = {CMD_PUSH};
    char *dir_str = strtok(NULL, " ");

    if (!is_valid_direction(dir_str)){
        handle_error("Invalid direction in PUSH command", line_number);
        return (Command){CMD_UNKNOWN};
    }

    cmd.parametrs.direction = parse_direction(dir_str);

    return cmd;
}

Command parse_load_command(int line_number){
    Command cmd = {0};
    cmd.type = CMD_LOAD;
    cmd.line_number = line_number;

    char *filename = strtok(NULL, " ");

    if (!filename || strlen(filename) == 0){
        handle_error("Missing filename in LOAD command", line_number);
        return (Command){CMD_UNKNOWN};
    } else if (strlen(filename) >= MAX_FILENAME) {
        handle_error("Filename too long in LOAD command", line_number);
        return (Command){CMD_UNKNOWN};
    }

    strncpy(cmd.parametrs.filename, filename, MAX_FILENAME - 1);
    cmd.parametrs.filename[MAX_FILENAME - 1] = '\0';
    return cmd;
}

Command parse_if_command(int line_number){

    Command cmd = {0};
    cmd.type = CMD_IF;
    cmd.line_number = line_number;

    char* token;

    token = strtok(NULL, " \t\r\n");
    if (!token || strcmp(token, "CELL") != 0) {
        handle_error("Expected 'CELL' after IF", line_number);
        cmd.type = CMD_UNKNOWN;
        return cmd;
    }

    char* x_str = strtok(NULL, " \t\r\n");
    char* y_str = strtok(NULL, " \t\r\n");
    if (!x_str || !y_str) {
        handle_error("Missing coordinates in IF", line_number);
        cmd.type = CMD_UNKNOWN;
        return cmd;
    }

    int x, y;
    if (parse_strict_positive_integer(x_str, &x) == -1 ||
        parse_strict_positive_integer(y_str, &y) == -1) {
        handle_error("Invalid coordinates in IF", line_number);
        cmd.type = CMD_UNKNOWN;
        return cmd;
    }

    token = strtok(NULL, " \t\r\n");
    if (!token || strcmp(token, "IS") != 0) {
        handle_error("Expected 'IS' in IF", line_number);
        cmd.type = CMD_UNKNOWN;
        return cmd;
    }

    char* symbol_str = strtok(NULL, " \t\r\n");
    if (!symbol_str || strlen(symbol_str) != 1 || !is_valid_ceil_symbol(symbol_str[0])) {
        handle_error("Invalid symbol in IF", line_number);
        cmd.type = CMD_UNKNOWN;
        return cmd;
    }

    char expected = symbol_str[0];

    token = strtok(NULL, " \t\r\n");
    if (!token || strcmp(token, "THEN") != 0) {
        handle_error("Expected 'THEN' in IF", line_number);
        cmd.type = CMD_UNKNOWN;
        return cmd;
    }

    char* then_line = strtok(NULL, "\r\n"); 
    if (!then_line) {
        handle_error("Missing command after THEN", line_number);
        cmd.type = CMD_UNKNOWN;
        return cmd;
    }

    while (*then_line == ' ' || *then_line == '\t') then_line++;
    if (*then_line == '\0') {
        handle_error("Empty command after THEN", line_number);
        cmd.type = CMD_UNKNOWN;
        return cmd;
    }

    Command then_cmd = parse_command(then_line, line_number);
    if (then_cmd.type == CMD_UNKNOWN) {
        cmd.type = CMD_UNKNOWN;
        return cmd;
    }

    cmd = parse_command(then_line, line_number);
    return cmd;
}