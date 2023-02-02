#include <stdio.h>
#include <stdlib.h>
#include "pacman.h"
#include "map.h"
#include "movement.h"

GAME game;

int get_file_length(FILE* file) {
    fseek(file, 0, SEEK_END);
    int length = ftell(file);

    return length;
}


void set_up() {
    read_map(&game.map);
}

void clean_up() {
    deallocate_map(&game.map);
}

int game_finished() {
    return game.gameover;
}

void move_ghosts() {
    for (int i = 0; i < game.map.ghost_count; i++)
    {
        move_ghost_random(&game, i, 1000);
    }
}

// GHOST* get_ghost_in_position(POSITION_2D position) {
//     for (int i = 0; i < game.map.ghost_count; i++)
//     {
//         POSITION_2D ghost_position = game.map.ghosts[i].position;

//         if (ghost_position.x == position.x && ghost_position.y == position.y)
//         {
//             return &game.map.ghosts[i];
//         }
//     }

//     return NULL;
// }

void destroy_ghost_at(POSITION_2D position) {
    game.map.view[position.y][position.x] = FREE_SPACE_SYMBOL;
    find_ghosts(&game.map);
}

int detonate_bomb() {
    if (game.pills <= 0) return 0;
    int y = game.player_position.y;
    int x = game.player_position.x;

    POSITION_2D neighbors[4] = {
        {x, y + 1},
        {x + 1, y},
        {x, y - 1},
        {x - 1, y},
    };

    char** map = game.map.view;

    for (int i = 0; i < 4; i++)
    {
        POSITION_2D position = neighbors[i];
        if (map[position.y][position.x] == GHOST_SYMBOL)
        {
            destroy_ghost_at(position);
        }
    }

    game.pills--;

    return 1;
}

int process_command(char command) {
    if (command == DETONATE_BOMB) return detonate_bomb();

    return try_to_move_player(&game, command);
}

void game_loop() {
    game.gameover = 0;
    game.pills = 0;
    find_player(&game.map, &game.player_position);
    find_ghosts(&game.map);

    do
    {
        char command;
        scanf(" %c", &command);
        process_command(command);
        move_ghosts();
        render(&game.map);
    } while (!game_finished());

    printf("\nGAME OVER\n\nYou were eaten.\n");
}

int main() {
    set_up();
    game_loop();
    clean_up();
}