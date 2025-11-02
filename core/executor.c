// executor.c

#include "executor.h"
#include "../data/structures.h"
#include "../utils/errors.h"
#include "../utils/validator.h"
#include "../data/field.h"
#include "../parser/parser.h"

#include <stdio.h>
#include <stdlib.h>


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

    
    if (!is_empty_cell(field, x, y))
        handle_error("Start position must be empty", line_number);

    
    field->dino_pos.x = x;
    field->dino_pos.y = y;
    field->dino_placed = 1;
    field->cells[x][y].symbol = DINOSAUR;

    return 1; // Динозавр посажен в клетку
}

int execute_move(Field *field, Direction direction, int line_number){
    if (!field->dino_placed)
        handle_error("Cannot move dinosaur before START", line_number);

    
    int new_x = field->dino_pos.x;
    int new_y = field->dino_pos.y;
    
    switch (direction) {
        case DIR_UP:    (new_y--) % field->height; break; // Для тора
        case DIR_DOWN:  (new_y++) % field->height; break;
        case DIR_LEFT:  (new_x--) % field->width; break;
        case DIR_RIGHT: (new_x++) % field->width; break;
        default: return 0; // По идее такого не будет, просто на всякий, ввод направления проверяется
    }
    
    if (!are_valid_coordinates(new_x, new_y, field->width, field->height)){
        handle_warning("Movement would go out of bounds", line_number); // Такого тоже не будет, динозавр гуляет по тору
        return 0;
    }

    if (is_hole(field, new_x, new_y))
        handle_error("Dinosaur fell into a hole!", line_number);
    
    if (is_obstacle(field, new_x, new_y)) {
        handle_warning("Cannot move to obstacle tile", line_number);
        return 0;
    }
    
    field->cells[field->dino_pos.y][field->dino_pos.x].symbol = EMPTY_TILE;
    field->dino_pos.x = new_x;
    field->dino_pos.y = new_y;
    field->cells[new_y][new_x].symbol = DINOSAUR;
    
    return 1; // Динозавр пошёл
}

int execute_paint(Field *field, char color, int line_number){
    if (!field->dino_placed)
        handle_error("Cannot paint before dinosaur placement", line_number);
        
    
    field->cells[field->dino_pos.y][field->dino_pos.x].symbol = color;
    field->cells[field->dino_pos.y][field->dino_pos.x].base_symbol = color;
    field->cells[field->dino_pos.y][field->dino_pos.x].is_colored = 1;
    
    return 1; // Клетка накрашена (Куда это она так)
}
int execute_jump(Field *field, Direction direction, int distance, int line_number) {
    if (!field->dino_placed || distance <= 0)
        handle_error("Invalid jump parameters", line_number);
    
    int new_x = field->dino_pos.x;
    int new_y = field->dino_pos.y;
    
    for (int i = 0; i < distance; i++){
        switch (direction) {
            case DIR_UP:    (--new_y) % field->height; break; // Для тора
            case DIR_DOWN:  (++new_y) % field->height; break;
            case DIR_LEFT:  (--new_x) % field->width; break;
            case DIR_RIGHT: (++new_x) % field->width; break;
            default: return 0; // По идее такого не будет, просто на всякий, ввод направления проверяется
        }

        if (is_obstacle(field, new_x, new_y)) {
            handle_warning("Cannot move to obstacle cell", line_number);
            return 0;
        } // надо написать такую же для других клеток с предметами
        

        field->cells[field->dino_pos.y][field->dino_pos.x].symbol 
                = field->cells[field->dino_pos.y][field->dino_pos.x].base_symbol;
        field->dino_pos.x = new_x;
        field->dino_pos.y = new_y;
        field->cells[new_y][new_x].symbol = DINOSAUR;
    }

    if (is_hole(field, new_x, new_y)) {
        handle_error("Dinosaur fell into a hole!", line_number);
    }
    

    field->cells[field->dino_pos.y][field->dino_pos.x].symbol 
        = field->cells[field->dino_pos.y][field->dino_pos.x].base_symbol;
    field->dino_pos.x = new_x;
    field->dino_pos.y = new_y;
    field->cells[new_y][new_x].symbol = DINOSAUR;
    
    return 1;
}

int execute_dig(Field *field, Direction direction, int line_number);
int execute_mound(Field *field, Direction direction, int line_number);
int execute_grow(Field *field, Direction direction, int line_number);
int execute_cut(Field *field, Direction direction, int line_number);
int execute_make(Field *field, Direction direction, int line_number);
int execute_push(Field *field, Direction direction, int line_number);
