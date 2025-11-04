// field.c

#include "structures.h"
#include "field.h"
#include "../utils/common.h"
#include <stdio.h>
#include <stdlib.h>
#include "../utils/errors.h"

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

    field->tiles = malloc(height * sizeof (Tile*));
    for (int i = 0; i < height; ++i) {
        field->tiles[i] = malloc(width * sizeof (Tile));
        for (int j = 0; j < width; j++){
            field->tiles[i][j].symbol = EMPTY_TILE;
            field->tiles[i][j].base_symbol = EMPTY_TILE;
            field->tiles[i][j].is_colored = 0;
            field->tiles[i][j].is_dino_placed = 0;
        }
    }

    return field;
}

void free_field(Field* field){
    if(!field) return;

    for (int i = 0; i < field->height; ++i) {
        free(field->tiles[i]);
    }
    free(field->tiles);
    free(field);
}


void print_field(const Field *field){
    for (int i = 0; i < field->width - 4; i++){
        printf("_");
    }
    printf("DINO-FLEX");
    for (int i = 0; i < field->width - 4; i++){
        printf("_");
    }
    printf("\n");

    for (int i = 0; i < field->height; ++i) {
        for (int j = 0; j < field->width; ++j)
            if (field->tiles[i][j].is_dino_placed){
                printf("%c ", DINOSAUR);
            } else if (field->tiles[i][j].symbol == EMPTY_TILE){
                printf("%c ", field->tiles[i][j].base_symbol);
            } else{
                printf("%c ", field->tiles[i][j].symbol);
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
    return field->tiles[y][x].symbol == EMPTY_TILE;
}

int is_obstacle(const Field* field, int x, int y) {
    if (!is_valid_position(field, x, y)) return 0;
    char c = field->tiles[y][x].symbol;
    return c == MOUNTAIN || c == TREE || c == ROCK;
}

int is_mountain(const Field* field, int x, int y) {
    if (!is_valid_position(field, x, y)) return 0;
    return field->tiles[y][x].symbol == MOUNTAIN;
}

int is_hole(const Field* field, int x, int y) {
    if (!is_valid_position(field, x, y)) return 0;
    return field->tiles[y][x].symbol == HOLE;
}

int is_tree(const Field* field, int x, int y) {
    if (!is_valid_position(field, x, y)) return 0;
    return field->tiles[y][x].symbol == TREE;
}

int is_rock(const Field* field, int x, int y) {
    if (!is_valid_position(field, x, y)) return 0;
    return field->tiles[y][x].symbol == ROCK;
}

int set_tile(Field* field, int x, int y, char symbol, int preserve_color) {
    if (!is_valid_position(field, x, y)) return 0;
    
    if (preserve_color && field->tiles[y][x].is_colored)
        field->tiles[y][x].base_symbol = symbol;
    else
        field->tiles[y][x].symbol = symbol;
    return 1;
}

int save_field(const Field* field, const char* filename) {
    FILE* file = fopen(filename, "w");
    if (!file)
        handle_error("Cannot create output file", 0);
        // Записываем поле в файл
    for (int y = 0; y < field->height; y++) {
        for (int x = 0; x < field->width; x++) {
            if (field->tiles[y][x].is_dino_placed == 1){
                fputc(DINOSAUR, file);
                fputc(' ', file);
            } else if (field->tiles[y][x].symbol == EMPTY_TILE){
                fputc(field->tiles[y][x].base_symbol, file);
                fputc(' ', file);
            } else{
                fputc(field->tiles[y][x].symbol, file);
                fputc(' ', file);
            }
            
            
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
            char c = field->tiles[i][j].symbol;
            
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