// executor.c

#include "executor.h"
#include "../data/structures.h"
#include "../utils/errors.h"
#include "../utils/validator.h"
#include "../data/field.h"
#include "../parser/parser.h"
#include "../utils/common.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int execute_size(Field** field, int width, int height, int line_number) {
    if (*field)
        handle_error("SIZE command can only be used once", line_number);
    
    
    *field = create_field(width, height);
    if (!*field)
        handle_error("Failed to create field", line_number);

    return 1; // Поле создано
}

int execute_start(Field *field, int x, int y, int line_number){
    if (!field)
        handle_error("START must come after SIZE", line_number);
    
    if (!are_valid_coordinates(x, y, field->width, field->height))
        handle_error("Invalid start position", line_number);

    
    if (!is_empty_tile(field, x, y))
        handle_error("Start position must be empty", line_number);

    
    field->dino_pos.x = x;
    field->dino_pos.y = y;
    field->dino_placed = 1;
    field->tiles[y][x].is_dino_placed = 1;

    return 1; // Динозавр посажен в клетку
}

int execute_move(Field *field, Direction direction, int line_number){
    if (!field)
        handle_error("MOVE must come after SIZE", line_number);
    if (!field->dino_placed)
        handle_error("Cannot move dinosaur before START", line_number);

    
    int new_x = field->dino_pos.x;
    int new_y = field->dino_pos.y;
    
    switch (direction) {
            case DIR_UP:    new_y = (field->height + new_y - 1) % field->height; break; // Для тора
            case DIR_DOWN:  new_y = (field->height + new_y + 1) % field->height; break;
            case DIR_LEFT:  new_x = (field->width + new_x - 1) % field->width; break;
            case DIR_RIGHT: new_x = (field->width + new_x + 1) % field->width; break;
            default: return 0; // По идее такого не будет, просто на всякий, ввод направления проверяется
        }
    
    if (!are_valid_coordinates(new_x, new_y, field->width, field->height)){
        handle_warning("Movement would go out of bounds", line_number); // Такого тоже не будет, динозавр гуляет по тору (бог грома не доволен)
        return 0;
    }

    if (is_hole(field, new_x, new_y))
        handle_error("Dinosaur fell into a hole!", line_number);
    
    if (is_obstacle(field, new_x, new_y)) {
        handle_warning("Cannot move to obstacle tile", line_number);
        return 1;
    }
    
    field->tiles[field->dino_pos.y][field->dino_pos.x].is_dino_placed = 0;
    field->dino_pos.x = new_x;
    field->dino_pos.y = new_y;
    field->tiles[new_y][new_x].is_dino_placed = 1;
    
    return 1; // Динозавр пошёл
}

int execute_paint(Field *field, char color, int line_number){
    if (!field)
        handle_error("PAINT must come after SIZE", line_number);
    if (!field->dino_placed)
        handle_error("Cannot paint before dinosaur placement", line_number);

    field->tiles[field->dino_pos.y][field->dino_pos.x].base_symbol = color;
    
    return 1; // Клетка накрашена (Куда это она собралась?)
}

int execute_jump(Field *field, Direction direction, int distance, int line_number) {
    if (!field)
        handle_error("JUMP must come after SIZE", line_number);
    if (!field->dino_placed)
        handle_error("Cannot jump before dinosaur placement", line_number);
 
    int new_x = field->dino_pos.x;
    int new_y = field->dino_pos.y;
    
    for (int i = 0; i < distance; ++i){
        switch (direction) {
            case DIR_UP:    new_y = (field->height + new_y - 1) % field->height; break; // Для тора
            case DIR_DOWN:  new_y = (field->height + new_y + 1) % field->height; break;
            case DIR_LEFT:  new_x = (field->width + new_x - 1) % field->width; break;
            case DIR_RIGHT: new_x = (field->width + new_x + 1) % field->width; break;
            default: return 0; // По идее такого не будет, просто на всякий, ввод направления проверяется
        }


        if (is_obstacle(field, new_x, new_y)) {
            handle_warning("Cannot jump on obstacle tile", line_number);
            return 1; // Умный в гору не пойдёт, умный гору обойдёт
        }
        
        // Проверка полёта, через что пргает динозавр, чтобы оставить в клетке нужный символ
        
        field->tiles[field->dino_pos.y][field->dino_pos.x].is_dino_placed = 0;

        field->dino_pos.x = new_x;
        field->dino_pos.y = new_y;

        field->tiles[new_y][new_x].is_dino_placed = 1;
    }

    if (is_hole(field, new_x, new_y)) {
        handle_error("Dinosaur fell into a hole!", line_number);
    }
    
    return 1;
}

int execute_dig(Field *field, Direction direction, int line_number){
    if (!field)
        handle_error("DIG must come after SIZE", line_number);
    if (!field->dino_placed)
        handle_error("Cannot dig before dinosaur placement", line_number);
    
    
    int dig_x = field->dino_pos.x;
    int dig_y = field->dino_pos.y;
    
    switch (direction) {
            case DIR_UP:    dig_y = (field->height + dig_y - 1) % field->height; break; // Для тора
            case DIR_DOWN:  dig_y = (field->height + dig_y + 1) % field->height; break;
            case DIR_LEFT:  dig_x = (field->width + dig_x - 1) % field->width; break;
            case DIR_RIGHT: dig_x = (field->width + dig_x + 1) % field->width; break;
            default: return 0; // По идее такого не будет, просто на всякий, ввод направления проверяется
        }

    if (!is_empty_tile(field, dig_x, dig_y)){
        handle_warning("Cannot dig the tile", line_number);
        return 1;
    }

    field->tiles[dig_y][dig_x].symbol = HOLE;
    return 1; // Динозавр вскопал ямку
}


int execute_mound(Field *field, Direction direction, int line_number){
    if (!field)
        handle_error("MOUND must come after SIZE", line_number);
    if (!field->dino_placed)
        handle_error("Cannot mound before dinosaur placement", line_number);
    
    
    int mound_x = field->dino_pos.x;
    int mound_y = field->dino_pos.y;
    
    switch (direction) {
            case DIR_UP:    mound_y = (field->height + mound_y - 1) % field->height; break; // Для тора
            case DIR_DOWN:  mound_y = (field->height + mound_y + 1) % field->height; break;
            case DIR_LEFT:  mound_x = (field->width + mound_x - 1) % field->width; break;
            case DIR_RIGHT: mound_x = (field->width + mound_x + 1) % field->width; break;
            default: return 0; // По идее такого не будет, просто на всякий, ввод направления проверяется
        }

    if (is_hole(field, mound_x, mound_y)){
        field->tiles[mound_y][mound_x].symbol = EMPTY_TILE;
        return 1;
    } else if (!is_empty_tile(field, mound_x, mound_y)) {
        handle_warning("Cannot do the mountain in that tile", line_number);
        return 1;
    }  
    
    field->tiles[mound_y][mound_x].symbol = MOUNTAIN;
    return 1; // Динозавр насыпал гору
}

int execute_grow(Field *field, Direction direction, int line_number){
    if (!field)
        handle_error("GROW must come after SIZE", line_number);
    if (!field->dino_placed)
        handle_error("Cannot grow the tree before dinosaur placement", line_number);
    
    
    int grow_x = field->dino_pos.x;
    int grow_y = field->dino_pos.y;
    
    switch (direction) {
            case DIR_UP:    grow_y = (field->height + grow_y - 1) % field->height; break; // Для тора
            case DIR_DOWN:  grow_y = (field->height + grow_y + 1) % field->height; break;
            case DIR_LEFT:  grow_x = (field->width + grow_x - 1) % field->width; break;
            case DIR_RIGHT: grow_x = (field->width + grow_x + 1) % field->width; break;
            default: return 0; // По идее такого не будет, просто на всякий, ввод направления проверяется
        }
        
    if (!is_empty_tile(field, grow_x, grow_y)){
        handle_warning("Cannot do the grow the tree in that tile", line_number);
        return 1;
    }

    field->tiles[grow_y][grow_x].symbol = TREE;
    return 1;
}

int execute_cut(Field *field, Direction direction, int line_number){
    if (!field)
        handle_error("CUT must come after SIZE", line_number);
    if (!field->dino_placed)
        handle_error("Cannot cut the tree before dinosaur placement", line_number);
    
    
    int cut_x = field->dino_pos.x;
    int cut_y = field->dino_pos.y;
    
    switch (direction) {
            case DIR_UP:    cut_y = (field->height + cut_y - 1) % field->height; break; // Для тора
            case DIR_DOWN:  cut_y = (field->height + cut_y + 1) % field->height; break;
            case DIR_LEFT:  cut_x = (field->width + cut_x - 1) % field->width; break;
            case DIR_RIGHT: cut_x = (field->width + cut_x + 1) % field->width; break;
            default: return 0; // По идее такого не будет, просто на всякий, ввод направления проверяется
        }

    if (!is_tree(field, cut_x, cut_y)){
        handle_warning("Cannot do the cut the tree in that tile", line_number);
        return 1;
    }
    
    field->tiles[cut_y][cut_x].symbol = EMPTY_TILE;
    return 1;
}

int execute_make(Field *field, Direction direction, int line_number){
    if (!field)
        handle_error("MAKE must come after SIZE", line_number);
    if (!field->dino_placed)
        handle_error("Cannot make the rock before dinosaur placement", line_number);
    
    
    int grow_x = field->dino_pos.x;
    int grow_y = field->dino_pos.y;
    
    switch (direction) {
            case DIR_UP:    grow_y = (field->height + grow_y - 1) % field->height; break; // Для тора
            case DIR_DOWN:  grow_y = (field->height + grow_y + 1) % field->height; break;
            case DIR_LEFT:  grow_x = (field->width + grow_x - 1) % field->width; break;
            case DIR_RIGHT: grow_x = (field->width + grow_x + 1) % field->width; break;
            default: return 0; // По идее такого не будет, просто на всякий, ввод направления проверяется
        }
        
    if (!is_empty_tile(field, grow_x, grow_y)){
        handle_warning("Cannot make the rock in that tile", line_number);
        return 1;
    }

    field->tiles[grow_y][grow_x].symbol = ROCK;
    return 1;
}

int execute_push(Field *field, Direction direction, int line_number){
    if (!field)
        handle_error("MAKE must come after SIZE", line_number);
    if (!field->dino_placed)
        handle_error("Cannot make the rock before dinosaur placement", line_number);
    
    
    int rock_x = field->dino_pos.x;
    int rock_y = field->dino_pos.y;
    
    int push_x = field->dino_pos.x;
    int push_y = field->dino_pos.y;

    switch (direction) {
            case DIR_UP:
                rock_y = (field->height + push_y - 1) % field->height; 
                push_y = (field->height + push_y - 2) % field->height; 
                break; // Для тора
            case DIR_DOWN:
                rock_y = (field->height + push_y + 1) % field->height;
                push_y = (field->height + push_y + 2) % field->height; 
                break;
            case DIR_LEFT:
                rock_x = (field->width + push_x - 1) % field->width;
                push_x = (field->width + push_x - 2) % field->width;
                break;
            case DIR_RIGHT:
                rock_x = (field->width + push_x + 1) % field->width;
                push_x = (field->width + push_x + 2) % field->width; 
                break;
            default: return 0; // По идее такого не будет, просто на всякий, ввод направления проверяется
        }
        
    if (!is_rock(field, rock_x, rock_y)){
        handle_warning("Cannot push the rock in that tile", line_number);
        return 1;
    } else if (is_hole(field, push_x, push_y)){
        field->tiles[push_y][push_x].symbol = EMPTY_TILE;
        field->tiles[rock_y][rock_x].symbol = EMPTY_TILE;
        return 1;
    } else if (!is_empty_tile(field, push_x, push_y)){
        handle_warning("Cannot push the rock in that tile", line_number);
        return 1;
    }

    field->tiles[push_y][push_x].symbol = ROCK;
    field->tiles[rock_y][rock_x].symbol = EMPTY_TILE;
    return 1;
}

int execute_load(Field **field, const char *filename, int line_number) {
    if (*field) {
        handle_error("LOAD must be the first command", line_number);
        return 0;
    }

    FILE *file = fopen(filename, "r");
    if (!file) {
        handle_error("Failed to open file for LOAD", line_number);
        return 0;
    }

    // Определяем размеры из первой строки
    int width = 0, height = 0;
    char buffer[1024];
    if (fgets(buffer, sizeof(buffer), file)) {
        char *ptr = buffer;
        while (*ptr) {
            if (*ptr != ' ' && *ptr != '\n' && *ptr != '\r') width++;
            else if (*ptr == '\n') break;
            ptr++;
        }
        
        height = 1;
        while (fgets(buffer, sizeof(buffer), file)) {
            height++;
        }
    }
    rewind(file);

    // Создаём поле
    *field = create_field(width, height);
    if (!*field) {
        fclose(file);
        handle_error("Failed to create field in LOAD", line_number);
        return 0;
    }

    // Читаем построчно
    for (int y = 0; y < height; y++) {
        if (!fgets(buffer, sizeof(buffer), file)) break;
        char *token = strtok(buffer, " ");
        int x = 0;
        while (token && x < width) {
            char c = token[0];
            if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')){
                (*field)->tiles[y][x].base_symbol = c;
                (*field)->tiles[y][x].symbol = EMPTY_TILE;
            } else {
                (*field)->tiles[y][x].symbol = c;
                (*field)->tiles[y][x].base_symbol = EMPTY_TILE;
            }
            
            

            if (c == DINOSAUR) {
                (*field)->dino_pos.x = x;
                (*field)->dino_pos.y = y;
                (*field)->dino_placed = 1;
                (*field)->tiles[y][x].is_dino_placed = 1;
                (*field)->tiles[y][x].symbol = EMPTY_TILE;
            }
            token = strtok(NULL, " \t\r\n");
            x++;
        }
    }

    fclose(file);
    return 1;
}