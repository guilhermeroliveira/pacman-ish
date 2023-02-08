#ifndef _MAP_H
#define _MAP_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "entity.h"
#include "list.h"

#define PLAYER '@'
#define ENEMY 'E'
#define VERTICAL_WALL '|'
#define HORIZONTAL_WALL '-'
#define EMPTY_SPACE '.'
#define POWERUP 'P'

typedef struct map
{
   char **matrix;
   int rows;
   int columns;
   ENTITY *player;
   bool powerup;
   LIST *enemies;
} MAP;

/*
   Initializes the given map, populating its matrix, setting its dimensions and
   position's initial coordinates.
*/
void initialize_map(MAP *map);

/*
   Writes the map to the standard output.
*/
void print_map(MAP *map);

/*
   Checks if a movement to the specified coordinates is valid.
   A movement is valid if it fulfills the following requirements:

   - Is within the map boundaries
   - Does not overlap with a wall or with an alike entity.
*/
bool is_valid_movement(MAP *map, ENTITY *entity, int x, int y);

/*
   Move the given entity on the map, checking if a movement to
   the position (x, y) is valid on the given map, following the
   rules specified on is_valid_movement function.
*/
void move_entity(MAP *map, ENTITY *entity, int newX, int newY);

/*
   Check if the player still exists on the map.
*/
bool find_player(MAP *map);

/*
   Returns whether is a wall or not in the given coordinates.
*/
bool is_wall(MAP *map, int x, int y);

/*
   Returns whether there is an enemy or not in the given coordinates.
*/
bool is_enemy(MAP *map, int x, int y);

#endif