// specific_parser.c

#include "../utils/common.h"
#include "specific_parser.h"
#include "../utils/errors.h"
#include <stdio.h>
#include <string.h>
#include "../utils/validator.h"

Command parse_size_command(int line_number){
    Command cmd = {CMD_SIZE};
    char *width_str = strtok(NULL, " ");
    char *height_str = strtok(NULL, " ");

    int width, height;
    if (!parse_strict_integer(width_str, &width)){
        handle_error("Invalid width in SIZE command", line_number);
        return (Command){CMD_UNKNOWN};
    }

    if (!parse_strict_integer(height_str, &height)){
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
    Command cmd = {CMD_START};
    char* x_str = strtok(NULL, " ");
    char* y_str = strtok(NULL, " ");

    int x, y; 
    if (!parse_strict_integer(x_str, &x)){
        handle_error("Invalid X-coordinate in START command", line_number);
        return (Command){CMD_UNKNOWN};
    }

    if (!parse_strict_integer(y_str, &y)){
        handle_error("Invalid Y-coordinate in START command", line_number);
        return (Command){CMD_UNKNOWN};
    }

    cmd.parametrs.position.x = x;
    cmd.parametrs.position.y = y;

    return cmd;
}

Command parse_move_command(int line_number){
    Command cmd = {CMD_MOVE};
    char* dir_str = strtok(NULL, " ");

    if (!is_valid_direction(dir_str)){
        handle_error("Invalid direction in MOVE command", line_number);
        return (Command){CMD_UNKNOWN};
    }

    cmd.parametrs.direction = parse_direction(dir_str);
    return cmd;
}

Command parse_paint_command(int line_number){
    Command cmd = {CMD_PAINT};
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

Command parse_dig_command(int line_number){
    Command cmd = {CMD_DIG};
    char *dir_str = strtok(NULL, " ");

    if (!is_valid_direction(dir_str)){
        handle_error("Invalid direction in DIG command", line_number);
        return (Command){CMD_UNKNOWN};
    }

    cmd.parametrs.direction = parse_direction(dir_str);

    return cmd;
}

Command parse_mound_command(int line_number){
    Command cmd = {CMD_MOUND};
    char *dir_str = strtok(NULL, " ");

    if (!is_valid_direction(dir_str)){
        handle_error("Invalid direction in MOUND command", line_number);
        return (Command){CMD_UNKNOWN};
    }

    cmd.parametrs.direction = parse_direction(dir_str);

    return cmd;
}

Command parse_jump_command(int line_number){
    Command cmd = {CMD_JUMP};
    char *dir_str = strtok(NULL, " ");
    char *dist_str = strtok(NULL, " ");

    int distance;
    if (!is_valid_direction(dir_str)){
        handle_error("Invalid direction in JUMP command", line_number);
        return (Command){CMD_UNKNOWN};
    }

    if (!parse_strict_integer(dist_str, &distance)) {
        handle_error("Invalid distance in JUMP command", line_number);
        return (Command){CMD_UNKNOWN};
    }

    cmd.parametrs.direction = parse_direction(dir_str);
    cmd.parametrs.jump_distance = distance;

    return cmd;
}

Command parse_grow_command(int line_number){
    Command cmd = {CMD_GROW};
    char *dir_str = strtok(NULL, " ");

    if (!is_valid_direction(dir_str)){
        handle_error("Invalid direction in MOUND command", line_number);
        return (Command){CMD_UNKNOWN};
    }

    cmd.parametrs.direction = parse_direction(dir_str);

    return cmd;
}

Command parse_cut_command(int line_number){
    Command cmd = {CMD_CUT};
    char *dir_str = strtok(NULL, " ");

    if (!is_valid_direction(dir_str)){
        handle_error("Invalid direction in CUT command", line_number);
        return (Command){CMD_UNKNOWN};
    }

    cmd.parametrs.direction = parse_direction(dir_str);

    return cmd;
}

Command parse_make_command(int line_number){
    Command cmd = {CMD_MAKE};
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