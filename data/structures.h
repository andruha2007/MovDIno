// structures.h

#ifndef STRUCTURES_H
#define STRUCTURES_H

#define DINOSAUR '#'
#define EMPTY_TILE '_'
#define MOUNTAIN '^'
#define HOLE '%'
#define ROCK '@'
#define TREE '&'

typedef struct { // Структура позиция динозавра
    int x;
    int y;
} Position;

typedef enum { // Обозначение направления
    DIR_UP,
    DIR_DOWN,
    DIR_LEFT,
    DIR_RIGHT,
    DIR_NONE
} Direction;

typedef struct {
    char symbol; // Символ, который находится в клетке в моменте
    char base_symbol; // Для крашенной клетки
    int is_dino_placed;
} Tile;

#endif