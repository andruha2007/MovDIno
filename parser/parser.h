// parser.h

#ifndef PARSER_H
#define PARSER_H

#include "../utils/common.h"
#include "../data/structures.h"

typedef enum {
    CMD_SIZE,
    CMD_START,
    CMD_MOVE,
    CMD_PAINT,
    CMD_DIG,
    CMD_MOUND,
    CMD_JUMP,
    CMD_GROW,
    CMD_CUT,
    CMD_MAKE,
    CMD_PUSH,
    CMD_EXEC,
    CMD_LOAD,
    CMD_UNDO,
    CMD_IF,
    CMD_UNKNOWN,
    CMD_COMMENT
} CommandType;


typedef struct {
    CommandType type;
    int line_number;
    int jump_distance;
    union {
        Direction direction;

        struct{
            int width;
            int height;
        } size;

        Position position;

        char filename[MAX_FILENAME]; // EXEC LOAD
        char color_char;
    } parametrs;

} Command;


typedef struct {
    Command *commands;
    int count;           // Текущее количество команд
    int capacity;        // Вместимость массива
} CommandList;

Direction parse_direction(const char* dir_str);

Command parse_command(const char *line, int line_number);

CommandList *create_command_list(void);

void free_command_list(CommandList* list);

void add_command(CommandList* list, Command cmd);

CommandList *parse_file(const char *filename);

int is_comment_line(const char *line);

#endif