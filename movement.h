
#ifndef MOVEMENT_H_DEFINED
#define MOVEMENT_H_DEFINED 1

#include "pacman.h"
#include "map.h"

#define UP 'w'
#define LEFT 'a'
#define DOWN 's'
#define RIGHT 'd'
#define DETONATE_BOMB 'b'


int move_with_deltas(GAME* game, POSITION_2D* player_position, char CHAR, int delta_y, int delta_x);
int move(GAME* game, POSITION_2D* position, char direction, char CHAR);
char get_char_at_direction(GAME* game, POSITION_2D* position, char direction);
int player_hit_ghost(GAME* game, char direction);
int ghost_hit_player(GAME* game, POSITION_2D* ghost_position, char direction);
int try_to_move_player(GAME* game, char direction);
int try_to_move_ghost(GAME* game, POSITION_2D* ghost_position, char direction);
int move_ghost_random(GAME* game, int ghost_index, int max_attempts);

#endif