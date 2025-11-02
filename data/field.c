// field.c

#include "structures.h"
#include "field.h"
#include "../utils/common.h"
#include <stdio.h>
#include <stdlib.h>

Field* create_field(int width, int height){
    if (width < MIN_WIDTH || width > MAX_WIDTH ||
    height < MIN_HEIGHT || height > MAX_HEIGHT){
        return NULL;
    }

    Field *field = malloc(sizeof(Field));
    field->width = width;
    field->height = height;
    field->dino_placed = 0;
    field->dino_pos.x = -1;
    field->dino_pos.y = -1;

    field->cells = malloc(height * sizeof (Tile*));
    for (int i = 0; i < height; ++i) {
        field->cells[i] = malloc(width * sizeof (Tile));
        for (int j = 0; j < width; j++){
            field->cells[i][j].symbol = EMPTY_TILE;
            field->cells[i][j].base_symbol = EMPTY_TILE;
            field->cells[i][j].is_colored = 0;
        }
    }

    return field;
}

void free_field(Field* field){
    if(!field) return;

    for (int i = 0; i < field->height; ++i) {
        free(field->cells[i]);
    }
    free(field->cells);
    free(field);
}


void print_field(const Field *field){
    printf("__________DINO-FLEX__________\n");

    for (int i = 0; i < field->height; ++i) {
        for (int j = 0; j < field->width; ++j)
            if (field->cells[i][j].symbol == EMPTY_TILE){
                printf("%c ", field->cells[i][j].base_symbol);
            } else{
                printf("%c ", field->cells[i][j].symbol);
            }
        printf("\n");
    }
    printf("\n");
}


int is_valid_position(const Field* field, int x, int y) {
    return (x >= 0 && x < field->width && y >= 0 && y < field->height);
}

int is_empty_tile(const Field* field, int x, int y) {
    if (!is_valid_position(field, x, y)) return 0;
    return field->cells[y][x].symbol == EMPTY_TILE;
}

int is_obstacle(const Field* field, int x, int y) {
    if (!is_valid_position(field, x, y)) return 0;
    char c = field->cells[y][x].symbol;
    return c == MOUNTAIN || c == TREE || c == ROCK;
}

int is_mountain(const Field* field, int x, int y) {
    if (!is_valid_position(field, x, y)) return 0;
    return field->cells[y][x].symbol == MOUNTAIN;
}

int is_hole(const Field* field, int x, int y) {
    if (!is_valid_position(field, x, y)) return 0;
    return field->cells[y][x].symbol == HOLE;
}

int is_tree(const Field* field, int x, int y) {
    if (!is_valid_position(field, x, y)) return 0;
    return field->cells[y][x].symbol == TREE;
}

int is_rock(const Field* field, int x, int y) {
    if (!is_valid_position(field, x, y)) return 0;
    return field->cells[y][x].symbol == ROCK;
}

int set_cell(Field* field, int x, int y, char symbol, int preserve_color) {
    if (!is_valid_position(field, x, y)) return 0;
    
    if (preserve_color && field->cells[y][x].is_colored)
        field->cells[y][x].base_symbol = symbol;
    else
        field->cells[y][x].symbol = symbol;
    return 1;
}

int save_field(const Field* field, const char* filename) {
    FILE* file = fopen(filename, "w");
    if (!file)
        handle_error("Cannot create output file", 0);
        // Записываем поле в файл
    for (int y = 0; y < field->height; y++) {
        for (int x = 0; x < field->width; x++) {
            fputc(field->cells[y][x].symbol, file);
        }
        fputc('\n', file); // Новая строка после каждой строки поля
    }

    fclose(file);
    return 1;
}

#if 0
void print_field(const Field* field) {
    printf("\n");
    for (int i = 0; i < field->height; i++) {
        printf(COLOR_CYAN "%2d " COLOR_RESET, i);
        for (int j = 0; j < field->width; j++) {
            char c = field->cells[i][j].symbol;
            
            if (c == '#') printf(COLOR_BRIGHT_GREEN STYLE_BOLD "%c " COLOR_RESET, c);
            else if (c == '%') printf(COLOR_RED STYLE_BOLD "%c " COLOR_RESET, c);
            else if (c == '^') printf(COLOR_YELLOW "%c " COLOR_RESET, c);
            else if (c == '&') printf(COLOR_GREEN "%c " COLOR_RESET, c);
            else if (c == '@') printf(COLOR_CYAN "%c " COLOR_RESET, c);
            else if (c >= 'a' && c <= 'z') printf(COLOR_BRIGHT_BLUE "%c " COLOR_RESET, c);
            else printf("%c ", c);
        }
        printf("\n");
    }
    printf("\n");
}
#endif