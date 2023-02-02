#ifndef MAP_H_DEFINED
#define MAP_H_DEFINED 1

#include <stdio.h>

#define VERTICAL_WALL_SYMBOL '|'
#define HORIZONTAL_WALL_SYMBOL '-'
#define PLAYER_SYMBOL '@'
#define FREE_SPACE_SYMBOL '.'
#define GHOST_SYMBOL 'y'
#define PILL_SYMBOL 'P'
#define ERROR_SYMBOL 'E'

struct size_2d {
    int width, height;
};

struct position_2d {
    int x, y;
};

typedef struct position_2d POSITION_2D;

struct ghost
{
    POSITION_2D position;
};

typedef struct ghost GHOST;


struct map {
    char** view;
    struct size_2d size;
    GHOST* ghosts;
    int ghost_count;
};

typedef struct size_2d SIZE_2D;
typedef struct map MAP;

struct size_2d get_size(FILE* file);
void allocate_map(MAP* map);
void deallocate_map(MAP* map);
void read_map(MAP* map);
void render(MAP* map);
void find_ghost_count(MAP* map);
void find_ghosts(MAP* map);
void find_player(MAP* map, POSITION_2D* player_position);

#endif