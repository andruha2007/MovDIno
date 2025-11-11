#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char symbol; // Символ, который находится в клетке в моменте
    char base_symbol; // Для крашенной клетки
    int is_dino_placed;
} Tile;

typedef struct { // Структура позиция динозавра
    int x;
    int y;
} Position;

typedef struct {
    int width;
    int height;
    Tile **tiles;
    Position dino_pos;
    int dino_placed;
} Field;

Field* create_field(int width, int height){
    if (width < 10 || width > 100 ||
    height < 10 || height > 100){
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
            field->tiles[i][j].symbol = '_';
            field->tiles[i][j].base_symbol = '_';
            field->tiles[i][j].is_dino_placed = 0;
        }
    }

    return field;
}

Field* deep_copy_field(const Field* src) {
    if (!src) return NULL;

    Field* dst = malloc(sizeof(Field));
    if (!dst) return NULL;

    // Копируем все поля
    dst->width = src->width;
    dst->height = src->height;
    dst->dino_pos.x = src->dino_pos.x;
    dst->dino_pos.y = src->dino_pos.y;
    dst->dino_placed = src->dino_placed;

    // Выделяем и копируем единый блок памяти
    size_t size = src->width * src->height;
    dst->tiles = malloc(size);
    if (!dst->tiles) {
        free(dst);
        return NULL;
    }
    memcpy(dst->tiles, src->tiles, size);

    return dst;
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
                printf("%c ", '@');
            } else if (field->tiles[i][j].symbol == '_'){
                printf("%c ", field->tiles[i][j].base_symbol);
            } else{
                printf("%c ", field->tiles[i][j].symbol);
            }
        printf("\n");
    }
    printf("\n");
}

int main(){
    Field *field = create_field(20, 10);

    Field *new_field = deep_copy_field(field);
    print_field(new_field);

    for (int i = 0; i < field->height; ++i) {
        free(field->tiles[i]);
    }
    free(field->tiles);
    free(field);
}


