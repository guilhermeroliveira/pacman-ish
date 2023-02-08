#include "map.h"

static void read_map_file(MAP *map);
static void find_enemies(MAP *map);
static bool is_inbound(MAP *map, int x, int y);
static bool is_valid_space(MAP *map, ENTITY *entity, int x, int y);
static bool is_alike_entity(MAP *map, ENTITY *entity, int x, int y);
static bool is_powerup(MAP *map, ENTITY *entity, int x, int y);
static void check_powerup(MAP *map, ENTITY *entity, int x, int y);

void initialize_map(MAP *map)
{
   map->enemies = malloc(sizeof(LIST));

   map->player = malloc(sizeof(ENTITY));
   map->player->representation = PLAYER;

   read_map_file(map);
}

bool find_player(MAP *map)
{
   for (int row = 0; row < map->rows; row++)
      for (int column = 0; column < map->columns; column++)
         if (map->matrix[row][column] == PLAYER)
         {
            map->player->x = column;
            map->player->y = row;

            return 1;
         }

   return 0;
}

static void find_enemies(MAP *map)
{
   for (int row = 0; row < map->rows; row++)
      for (int column = 0; column < map->columns; column++)
         if (map->matrix[row][column] == ENEMY)
         {
            ENTITY *enemyPosition = malloc(sizeof(ENTITY));
            enemyPosition->x = column;
            enemyPosition->y = row;
            enemyPosition->representation = 'E';

            add_item(map->enemies, enemyPosition);
         }
}

static void read_map_file(MAP *map)
{
   FILE *file = NULL;
   file = fopen("map.txt", "r");

   if (file == NULL)
   {
      puts("Error when opening map file, exitting...");
      exit(1);
   }

   fscanf(file, "%d %d", &(map->rows), &(map->columns));

   map->matrix = calloc(map->rows, sizeof(char *));

   for (int i = 0; i < map->rows; i++)
   {
      map->matrix[i] = calloc(map->columns + 1, sizeof(char));
      fscanf(file, "%11s", map->matrix[i]);
   }

   find_player(map);
   find_enemies(map);
}

static bool is_inbound(MAP *map, int x, int y)
{
   return (x > 0 && x < map->columns - 1) && (y > 0 && y < map->rows - 1);
}

bool is_wall(MAP *map, int x, int y)
{
   return map->matrix[y][x] == VERTICAL_WALL || map->matrix[y][x] == HORIZONTAL_WALL;
}

bool is_enemy(MAP *map, int x, int y)
{
   return map->matrix[y][x] == ENEMY;
}

static bool is_valid_space(MAP *map, ENTITY *entity, int x, int y)
{
   return !is_wall(map, x, y) &&
          map->matrix[y][x] != POWERUP;
}

static bool is_alike_entity(MAP *map, ENTITY *entity, int x, int y)
{
   return map->matrix[y][x] == entity->representation;
}

bool is_valid_movement(MAP *map, ENTITY *entity, int x, int y)
{
   return is_inbound(map, x, y) && (is_valid_space(map, entity, x, y) || is_powerup(map, entity, x, y)) && !is_alike_entity(map, entity, x, y);
}

void move_entity(MAP *map, ENTITY *entity, int newX, int newY)
{
   if (!is_valid_movement(map, entity, newX, newY))
      return;

   if (entity == map->player)
      check_powerup(map, entity, newX, newY);

   map->matrix[entity->y][entity->x] = EMPTY_SPACE;

   entity->x = newX;
   entity->y = newY;
   map->matrix[newY][newX] = entity->representation;
}

static bool is_powerup(MAP *map, ENTITY *entity, int x, int y)
{
   return entity != map->player ? false : map->matrix[y][x] == POWERUP;
}

static void check_powerup(MAP *map, ENTITY *entity, int x, int y)
{
   if (!is_powerup(map, entity, x, y))
      return;

   map->powerup = true;
}
