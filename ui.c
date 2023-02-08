#include <stdio.h>
#include <stdlib.h>

#include "ui.h"

char wall_sprite[4][7] = {
    {"......"},
    {"......"},
    {"......"},
    {"......"}};

char player_sprite[4][7] = {
    {" .--. "},
    {"/ _.-'"},
    {"\\  '-."},
    {" '--' "}};

char powerup_sprite[4][7] = {
    {"      "},
    {" .-.  "},
    {" '-'  "},
    {"      "}};

char empty_sprite[4][7] = {
    {"      "},
    {"      "},
    {"      "},
    {"      "}};

char enemy_sprite[4][7] = {
    {" .-.  "},
    {"| OO| "},
    {"|   | "},
    {"'^^^' "}};

static void print_sprite(char sprite[4][7], int spriteRow);

void print_map(MAP *map)
{
   puts("\e[1;1H\e[2J");

   for (int row = 0; row < map->rows; row++)
      for (int spriteRow = 0; spriteRow < 4; spriteRow++)
      {
         for (int col = 0; col < map->columns; col++)
         {
            switch (map->matrix[row][col])
            {
            case ENEMY:
               print_sprite(enemy_sprite, spriteRow);
               break;

            case PLAYER:
               print_sprite(player_sprite, spriteRow);
               break;

            case POWERUP:
               print_sprite(powerup_sprite, spriteRow);
               break;

            case EMPTY_SPACE:
               print_sprite(empty_sprite, spriteRow);
               break;

            case VERTICAL_WALL:
            case HORIZONTAL_WALL:
               print_sprite(wall_sprite, spriteRow);
               break;

            default:
               break;
            }
         }

         printf("\n");
      }

   printf("powerup: %s\n", map->powerup ? "YES" : "NO");
}

static void print_sprite(char sprite[4][7], int spriteRow)
{
   printf("%s", sprite[spriteRow]);
}