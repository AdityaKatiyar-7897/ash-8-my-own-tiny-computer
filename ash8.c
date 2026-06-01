#include <SDL.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

#define COLS 40
#define ROWS 25
#define SCALE 16

int main(void)
{
    uint8_t screen[COLS * ROWS] = {0};

    screen[500] = 'A';
    screen[501] = 'A';
    screen[502] = 'A';
    screen[503] = 'A';
    screen[504] = 'A';

    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window *window = SDL_CreateWindow(
        "ASH-8 Monitor",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        COLS * SCALE,
        ROWS * SCALE,
        0
    );

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    bool running = true;
    SDL_Event e;

   while (running) {
       while (SDL_PollEvent(&e)) {
           if (e.type == SDL_QUIT) running = false;
       }
   
       SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
       SDL_RenderClear(renderer);
   
       for (int i = 0; i < COLS * ROWS; i++) {
           if (screen[i] == 'A') {
               int col = i % COLS;
               int row = i / COLS;
   
               SDL_Rect r = {
                   col * SCALE,
                   row * SCALE,
                   SCALE,
                   SCALE
               };
   
               SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
               SDL_RenderFillRect(renderer, &r);
           }
       }
   
       SDL_RenderPresent(renderer);
   }
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
