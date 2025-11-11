// field.h

#ifndef FIELD_H
#define FIELD_H

#include "structures.h"

typedef struct {
    int width;
    int height;
    Tile **tiles;
    Position dino_pos;
    int dino_placed;
} Field;

Field* create_field(int width, int height);
Field* deep_copy_field(const Field* src);

void free_field(Field* field);
void print_field(const Field* field);

int is_valid_position(const Field* field, int x, int y);
int is_empty_tile(const Field* field, int x, int y);
int is_obstacle(const Field* field, int x, int y);
int is_mountain(const Field* field, int x, int y);
int is_hole(const Field* field, int x, int y);
int is_tree(const Field* field, int x, int y);
int is_rock(const Field* field, int x, int y);

int set_tile(Field* field, int x, int y, char symbol, int preserve_color);
int save_field(const Field* field, const char* filename);

void print_colored_field(const Field* field);

#endif

