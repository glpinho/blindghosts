#include <time.h>
#include <stdlib.h>
#include "movement.h"
#include "map.h"
#include "pacman.h"

char directions[4] = {UP, LEFT, DOWN, RIGHT};

int move_with_deltas(GAME* game, POSITION_2D* player_position, char CHAR, int delta_y, int delta_x) {
    int x = player_position->x;
    int y = player_position->y;

    game->map.view[y][x] = FREE_SPACE_SYMBOL;
    game->map.view[y + delta_y][x + delta_x] = CHAR;
    player_position->x = x + delta_x;
    player_position->y = y + delta_y;
    return 1;
}

int move(GAME* game, POSITION_2D* position, char direction, char CHAR) {
    if (direction == UP) return move_with_deltas(game, position, CHAR, -1, 0);
    if (direction == LEFT) return move_with_deltas(game, position, CHAR, 0, -1);
    if (direction == DOWN) return move_with_deltas(game, position, CHAR, 1, 0);
    if (direction == RIGHT) return move_with_deltas(game, position, CHAR, 0, 1);
    return 0;
}

char get_char_at_direction(GAME* game, POSITION_2D* position, char direction) {
    if (direction == UP) return game->map.view[position->y - 1][position->x];
    if (direction == LEFT) return game->map.view[position->y][position->x - 1];
    if (direction == DOWN) return game->map.view[position->y + 1][position->x];
    if (direction == RIGHT) return game->map.view[position->y][position->x + 1];
    return ERROR_SYMBOL;
}

int player_hit_ghost(GAME* game, char direction) {
    POSITION_2D* player_position = &game->player_position;
    move(game, player_position, direction, PLAYER_SYMBOL);
    game->map.view[player_position->y][player_position->x] = GHOST_SYMBOL;
    game->gameover = 1;
    return 1;
}

int ghost_hit_player(GAME* game, POSITION_2D* ghost_position, char direction) {
    move(game, ghost_position, direction, GHOST_SYMBOL);
    game->gameover = 1;
    return 1;
}

int eat_pill(GAME* game, char direction) {
    move(game, &game->player_position, direction, PLAYER_SYMBOL);
    game->pills++;
}

int try_to_move_player(GAME* game, char direction) {
    POSITION_2D* player_position = &game->player_position;

    char next_char = get_char_at_direction(game, player_position, direction);

    if (next_char == FREE_SPACE_SYMBOL) return move(game, player_position, direction, PLAYER_SYMBOL);
    if (next_char == GHOST_SYMBOL) return player_hit_ghost(game, direction);
    if (next_char == PILL_SYMBOL) return eat_pill(game, direction);
    if (next_char == VERTICAL_WALL_SYMBOL) return 0;
    if (next_char == HORIZONTAL_WALL_SYMBOL) return 0;
    
    return 0;
}

int try_to_move_ghost(GAME* game, POSITION_2D* ghost_position, char direction) {
    char next_char = get_char_at_direction(game, ghost_position, direction);

    if (next_char == FREE_SPACE_SYMBOL) return move(game, ghost_position, direction, GHOST_SYMBOL);
    if (next_char == PLAYER_SYMBOL) return ghost_hit_player(game, ghost_position, direction);
    if (next_char == GHOST_SYMBOL) return 0;
    if (next_char == VERTICAL_WALL_SYMBOL) return 0;
    if (next_char == HORIZONTAL_WALL_SYMBOL) return 0;
    if (next_char == PILL_SYMBOL) return 0;
    
    return 0;
}

int move_ghost_random(GAME* game, int ghost_index, int max_attempts)
{
    for (int j = 0; j < max_attempts; j++)
    {
        POSITION_2D* ghost_position = &game->map.ghosts[ghost_index].position;
        int seed = time(0) + ghost_index + 100 * j;
        srand(seed);
        int random_index = rand() % 4;
        int success = try_to_move_ghost(game, ghost_position, directions[random_index]);

        if (success) return 1;
    }

    return 0;
}