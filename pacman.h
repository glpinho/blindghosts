#ifndef PACMAN_H_DEFINED
#define PACMAN_H_DEFINED 1

#include <stdio.h>
#include "map.h"

int get_file_length(FILE* file);

void read_map();
void clean_up();
void set_up();
void move_ghosts();

struct game
{
    MAP map;
    POSITION_2D player_position;
    int gameover;
    int pills;
};

typedef struct game GAME;

#endif
