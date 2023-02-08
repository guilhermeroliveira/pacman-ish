#include <stdbool.h>
#include <stdio.h>
#include <time.h>

#include "map.h"
#include "ui.h"

#define MOVE_UP 'w'
#define MOVE_DOWN 's'
#define MOVE_LEFT 'a'
#define MOVE_RIGHT 'd'
#define POWERUP_ACTION 'b'

void read_input(MAP *map);
void move_enemies(MAP *map);
int get_random_movement(MAP *map, ENTITY *entity, int *movement);
void check_status(MAP *map);
void use_powerup(MAP *map);
void remove_enemy(MAP *map, int x, int y);
void explosion(MAP *map, int x, int y, int xModifier, int yModifier, int remainingExplosions);

int main(int argc, char **argv)
{
   srand(time(0));
   MAP *map = calloc(1, sizeof(MAP));

   initialize_map(map);

   while (true)
   {
      print_map(map);

      if (!find_player(map))
      {
         puts("You lose!");
         break;
      }

      read_input(map);
      move_enemies(map);
   }
}

/*
   Reads the next character waiting for a
   movement command:

   - 'w': Up
   - 's': Down
   - 'a': Left
   - 'd': Right
*/
void read_input(MAP *map)
{
   char direction;
   scanf(" %c", &direction);

   int nextX = map->player->x;
   int nextY = map->player->y;

   switch (direction)
   {
   case MOVE_UP:
      nextY--;
      break;
   case MOVE_DOWN:
      nextY++;
      break;
   case MOVE_LEFT:
      nextX--;
      break;
   case MOVE_RIGHT:
      nextX++;
      break;
   case POWERUP_ACTION:
      use_powerup(map);
      return;
   default:
      return;
   }

   move_entity(map, map->player, nextX, nextY);
}

void move_enemies(MAP *map)
{
   for (int i = 0; i < map->enemies->size; i++)
   {
      ENTITY *enemy = item_at(map->enemies, i);

      int *randomMovement = calloc(2, sizeof(int));
      if (get_random_movement(map, enemy, randomMovement))
         move_entity(map, enemy, randomMovement[0], randomMovement[1]);
   }
}

int get_random_movement(MAP *map, ENTITY *entity, int *movement)
{
   int possibleMovements[4][2] = {
       {entity->x - 1, entity->y},
       {entity->x + 1, entity->y},
       {entity->x, entity->y - 1},
       {entity->x, entity->y + 1}};

   int index = rand() % 4;

   int newX = possibleMovements[index][0];
   int newY = possibleMovements[index][1];

   for (int i = 0; i < 10; i++)
   {
      index = rand() % 4;
      newX = possibleMovements[index][0];
      newY = possibleMovements[index][1];

      if (is_valid_movement(map, entity, newX, newY))
      {
         movement[0] = newX;
         movement[1] = newY;
         return 1;
      }
   }

   return 0;
}

void use_powerup(MAP *map)
{
   if (!map->powerup)
      return;

   map->powerup = false;

   explosion(map, map->player->x, map->player->y, 1, 0, 3);
   explosion(map, map->player->x, map->player->y, 0, 1, 3);
   explosion(map, map->player->x, map->player->y, -1, 0, 3);
   explosion(map, map->player->x, map->player->y, 0, -1, 3);
}

void explosion(MAP *map, int x, int y, int xModifier, int yModifier, int remainingExplosions)
{
   if (remainingExplosions <= 0)
      return;

   int targetX = x + xModifier;
   int targetY = y + yModifier;
   if (is_wall(map, targetX, targetY))
      return;

   if (is_enemy(map, targetX, targetY))
      remove_enemy(map, targetX, targetY);

   explosion(map, targetX, targetY, xModifier, yModifier, --remainingExplosions);
}

void remove_enemy(MAP *map, int x, int y)
{
   for (NODE *node = map->enemies->first; node != NULL; node = node->next)
   {
      ENTITY *enemy = (ENTITY *)node->item;
      if (enemy->x == x && enemy->y == y)
      {
         remove_item(map->enemies, enemy);
         free(enemy);
         map->matrix[y][x] = '.';
         break;
      }
   }
}
