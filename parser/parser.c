// parser.c

#include "../utils/common.h"
#include <string.h>
#include "../utils/validator.h"
#include "specific_parser.h"
#include "../data/structures.h"
#include <stdio.h>
#include <stdlib.h>
#include "../utils/errors.h"



Direction parse_direction(const char* dir_str) {
    if (!dir_str) return DIR_NONE;
    if (strcmp(dir_str, "UP") == 0)    return DIR_UP;
    if (strcmp(dir_str, "DOWN") == 0)  return DIR_DOWN;
    if (strcmp(dir_str, "LEFT") == 0)  return DIR_LEFT;
    if (strcmp(dir_str, "RIGHT") == 0) return DIR_RIGHT;
    return DIR_NONE;
}

Command parse_command(const char *line, int line_number) {
    Command cmd;

    cmd.type = CMD_UNKNOWN;
    cmd.parametrs.direction = DIR_NONE;
 
    if (has_left_spaces(line)){
        handle_error("Left spaceces are not allowed", line_number);
        return cmd;
    }

    if (is_comment_line(line) || is_whitespace_only(line)){
        cmd.type = CMD_COMMENT;
        return cmd;
    }

    char copy[MAX_LINE_LENGTH];
    strncpy(copy, line, sizeof (copy) - 1);

    char *token = strtok(copy, " ");
    if (!token){
        cmd.type = CMD_UNKNOWN;
        return cmd;
    }

    if (strcmp(token, "SIZE") == 0)
        return parse_size_command(line_number);
    else if (strcmp(token, "START") == 0)
        return parse_start_command(line_number);
    else if (strcmp(token, "MOVE") == 0)
        return parse_move_command(line_number);
    else if (strcmp(token, "PAINT") == 0)
        return parse_paint_command(line_number);
    else if (strcmp(token, "JUMP") == 0)
        return parse_jump_command(line_number);
    else if (strcmp(token, "DIG") == 0)
        return parse_dig_command(line_number);
    else if (strcmp(token, "MOUND") == 0)
        return parse_mound_command(line_number);
    else if (strcmp(token, "GROW") == 0)
        return parse_grow_command(line_number);
    else if (strcmp(token, "CUT") == 0)
        return parse_cut_command(line_number);
    else if (strcmp(token, "MAKE") == 0)
        return parse_make_command(line_number);
    else if (strcmp(token, "PUSH") == 0)
        return parse_push_command(line_number);
    else if (strcmp(token, "LOAD") == 0)
        return parse_load_command(line_number);
    handle_error("Unknown command", line_number);
    return cmd;
}


CommandList* create_command_list(void) {
    CommandList* list = (CommandList*) malloc(sizeof(CommandList));
    if (!list) return NULL;
    
    list->count = 0;
    list->capacity = 10;
    list->commands = (Command*) malloc(list->capacity * sizeof(Command));
    
    return list;
}

void free_command_list(CommandList* list) {
    if (!list) return;
    
    free(list->commands);
    free(list);
}

void add_command(CommandList* list, Command cmd) {
    if (list->count >= list->capacity) {
        list->capacity *= 2;
        list->commands = (Command*)realloc(list->commands, list->capacity * sizeof(Command));
    }
    
    list->commands[list->count++] = cmd;
}

CommandList *parse_file(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("%s\n", filename);
        handle_error("Cannot open input file", 0);
        return NULL;
    }
    
    CommandList *list = create_command_list();
    if (!list) {
        fclose(file);
        return NULL;
    }
    int line_number = 0;
    char line[MAX_LINE_LENGTH] = {0};

    while (fgets(line, sizeof(line), file) != NULL) {
        line_number++;
        // Удаляем символ новой строки
        line[strcspn(line, "\n")] = '\0';
        
        Command cmd = parse_command(line, line_number);
        cmd.line_number = line_number;
        if (cmd.type != CMD_COMMENT && cmd.type != CMD_UNKNOWN) {
            // Добавляем команду в список
            if (list->count >= list->capacity) {
                list->capacity *= 2;
                list->commands = realloc(list->commands, list->capacity * sizeof(Command));
            }

            list->commands[list->count++] = cmd;
        } else if (cmd.type == CMD_UNKNOWN)
            handle_error("Invalid command", line_number);   
    }

    fclose(file);
    return list;
}

