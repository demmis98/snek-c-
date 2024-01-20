#define SDL_MAIN_HANDLED
#include "SDL2/SDL.h"
#include <chrono>
#include <cstdlib>

#include <stdio.h>

#define SCREEN_WIDTH 480
#define SCREEN_HEIGHT 368
#define BLOCK_SIDE 16


void init(int *player_x, int *player_y, int *player_l, bool *player, int *apple_x, int *apple_y, bool *dead);


int main(){
  bool running;

  //open window
  SDL_Init(SDL_INIT_EVERYTHING);
  SDL_Window *window = SDL_CreateWindow("snek", 64, 64, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
  SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);


  int world_width = SCREEN_WIDTH / BLOCK_SIDE;
  int world_height = SCREEN_HEIGHT / BLOCK_SIDE;

  uint32_t time_old = SDL_GetTicks(), time_new;

  int player_x, player_y, player_l;
  bool dead = false;
  int temp_direction = 0, direction = 0;
  int temp_x, temp_y;
  int apple_x, apple_y, apple_temp;
  bool apple_ate = true;

  bool player[world_height][world_width][3];

  SDL_Rect temp_rect;
  temp_rect.w = BLOCK_SIDE;
  temp_rect.h = BLOCK_SIDE;

  running = true;
  init(&player_x, &player_y, &player_l, &player[0][0][0], &apple_x, &apple_y, &dead);
  while(running){
    //events
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
      switch (event.type) {
        case SDL_KEYDOWN:
          switch (event.key.keysym.sym) {
            case SDLK_ESCAPE:
              running = false;
              break;
            case SDLK_UP:
                temp_direction = 0;
              break;
            case SDLK_RIGHT:
                temp_direction = 1;
              break;
            case SDLK_DOWN:
                temp_direction = 2;
              break;
            case SDLK_LEFT:
                temp_direction = 3;
              break;
            case SDLK_r:
              init(&player_x, &player_y, &player_l, &player[0][0][0], &apple_x, &apple_y, &dead);
          }
          break;
        case SDL_QUIT:
          running = false;
      }
    }

    if(!dead){
      time_new = SDL_GetTicks();
      if(time_new - time_old > 1000/20){
        if(player_x < 0 || player_y < 0
          || player_x >= world_width || player_y >= world_height)
          dead = true;

        switch(temp_direction){
          case 0:
            if(direction != 2)
              direction = temp_direction;
            break;
          case 1:
            if(direction != 3)
              direction = temp_direction;
            break;
          case 2:
            if(direction != 0)
              direction = temp_direction;
            break;
          case 3:
            if(direction != 1)
              direction = temp_direction;
        }

        if(!dead)
          switch(direction){
            case 0:
              player_y--;
              if(player[player_y][player_x][2])
                dead = true;
              player[player_y][player_x][2] = true;
              player[player_y][player_x][1] = true;
              player[player_y][player_x][0] = true;
              break;
            case 1:
              player_x++;
              if(player[player_y][player_x][2])
                dead = true;
              player[player_y][player_x][2] = true;
              player[player_y][player_x][1] = false;
              player[player_y][player_x][0] = false;
              break;
            case 2:
              player_y++;
              if(player[player_y][player_x][2])
                dead = true;
              player[player_y][player_x][2] = true;
              player[player_y][player_x][1] = true;
              player[player_y][player_x][0] = false;
              break;
            case 3:
              player_x--;
              if(player[player_y][player_x][2])
                dead = true;
              player[player_y][player_x][2] = true;
              player[player_y][player_x][1] = false;
              player[player_y][player_x][0] = true;
          }
        if(dead){
          if(player_x < 0)
            player_x = 0;
          if(player_y < 0)
            player_y = 0;
          if(player_x >= world_width)
            player_x = world_width - 1;
          if(player_y >= world_height)
            player_y = world_height - 1;
        }

        temp_x = player_x;
        temp_y = player_y;
        for(int i = 0; i < player_l; i++){
          if(player[temp_y][temp_x][1])
            if(player[temp_y][temp_x][0])
              temp_y++;
            else
              temp_y--;
          else
            if(player[temp_y][temp_x][0])
              temp_x++;
            else
              temp_x--;
        }


        if(!player[apple_y][apple_x][2]){
          player[temp_y][temp_x][2] = 0;
        }
        else {
          player_l++;
          while(player[apple_y][apple_x][2]){
            apple_x = rand() % world_width;
            apple_y = rand() % world_height;
          }
        }

        time_old = SDL_GetTicks();
      }
    }

    //render
    SDL_SetRenderDrawColor(renderer, 64, 0, 64, 255);
    SDL_RenderClear(renderer);
    
    temp_x = player_x;
    temp_y = player_y;
    for(int i = 0; i < player_l; i++){
      temp_rect.x = temp_x * BLOCK_SIDE;
      temp_rect.y = temp_y * BLOCK_SIDE;
      int temp_color = i * (255 / player_l);
      if(!dead)
        SDL_SetRenderDrawColor(renderer, 255, temp_color, 255, 255);
      else
        SDL_SetRenderDrawColor(renderer, temp_color, temp_color, 255, 255);
      SDL_RenderFillRect(renderer, &temp_rect);
        if(player[temp_y][temp_x][1])
          if(player[temp_y][temp_x][0])
            temp_y++;
          else
            temp_y--;
        else
          if(player[temp_y][temp_x][0])
            temp_x++;
          else
            temp_x--;
    }
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    temp_rect.x = apple_x * BLOCK_SIDE;
    temp_rect.y = apple_y * BLOCK_SIDE;
    SDL_RenderFillRect(renderer, &temp_rect);
    
    SDL_RenderPresent(renderer);
  }

  //end

  return 0;
}

void init(int *player_x, int *player_y, int *player_l, bool *player, int *apple_x, int *apple_y, bool *dead){
  int world_width = SCREEN_WIDTH / BLOCK_SIDE;
  int world_height = SCREEN_HEIGHT / BLOCK_SIDE;
  *player_x = world_width / 2;
  *player_y = world_height / 2;

  for(int j = 0; j < world_height; j++)
    for(int i = 0; i < world_width; i++)
      for(int w = 0; w < 3; w++)
        *(player + w + (i * 3) + (j * world_width * 3)) = false;

  for(int w = 0; w < 3; w++)
    *(player + w + (*player_x * 3) + (*player_y * world_width * 3)) = false;
  *player_l = 1;

  *apple_x = *player_x;
  *apple_y = *player_y;
  *dead = false;
}
