#include <stdlib.h>
#include "map.h"
#include "pacman.h"

SIZE_2D get_size(FILE* file) {
    SIZE_2D size;
    size.width = 0;
    size.height = 0;

    int file_length = get_file_length(file);
    fseek(file, 0, SEEK_SET);

    int y = 1;
    int x = 0;

    for (int i = 0; i < file_length; i++)
    {
        char c;
        fscanf(file, "%c", &c);

        int end_of_line = c == '\n';

        if (end_of_line)
        {
            y++;
            x = 0;
        }
        else
        {
            x++;
        }
    }

    size.width = x;
    size.height = y;

    return size;
}

void allocate_map(MAP* map) {
    char** _map = malloc(sizeof(char*) * map->size.height);

    for (int y = 0; y < map->size.height; y++)
    {
        _map[y] = malloc(sizeof(char) * map->size.width);
    }
    
    map->view = _map;
}

void deallocate_map(MAP* map) {
    for (int y = 0; y < map->size.height; y++)
    {
        free(map->view[y]);
    }

    free(map->view);
    free(map->ghosts);
}

void render(MAP* map) {
    system("clear");

    for (int y = 0; y < map->size.height; y++) {
        printf("%s\n", map->view[y]);
    }
}

void read_map(MAP* map) {
    FILE* file = fopen("map.txt", "r");

    if (file == 0)
    {
        printf("map.txt not found!");
        exit(1);
    }


    int y = 0;
    map->size = get_size(file);
    allocate_map(map);
    fseek(file, 0, SEEK_SET);

    for (int y = 0; y < map->size.height; y++)
    {
        fscanf(file, "%s", map->view[y]);
    }

    render(map);

    fclose(file);
}

void find_ghost_count(MAP* map) {
    int count = 0;

    for (int y = 0; y < map->size.height; y++)
    {
        for (int x = 0; x < map->size.width; x++)
        {
            if (map->view[y][x] == GHOST_SYMBOL) count++;
        }
    }

    map->ghost_count = count;
}

void find_ghosts(MAP* map) {
    find_ghost_count(map);

    if (map->ghosts != 0)
    {
        free(map->ghosts);
    }

    map->ghosts = malloc(map->ghost_count * sizeof(GHOST));

    int i = 0;

    for (int y = 0; y < map->size.height; y++)
    {
        for (int x = 0; x < map->size.width; x++)
        {
            if (map->view[y][x] == GHOST_SYMBOL)
            {
                GHOST ghost;
                ghost.position.x = x;
                ghost.position.y = y;
                map->ghosts[i] = ghost;
                i++;

                if (i > map->ghost_count) return;
            }
        }
    }
}

void find_player(MAP* map, POSITION_2D* player_position) {
    for (int j = 0; j < map->size.height; j++)
    {
        for (int i = 0; i < map->size.width; i++)
        {
            if (map->view[j][i] == PLAYER_SYMBOL)
            {
                player_position->x = i;
                player_position->y = j;
                return;
            }
        }
    }
}