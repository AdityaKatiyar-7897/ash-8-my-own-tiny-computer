#include <SDL.h>
#include <stdint.h>
#include <stdbool.h>

#define COLS 40
#define ROWS 25
#define SCALE 16

typedef struct{
	int x;
	int pc;
} CPU;


int main(void)
{
    uint8_t screen[COLS * ROWS] = {0};
    uint8_t rom[256] = {0};

    for (int i = 0 ; i <20 ; i++){
    	rom[i] = 1;
    }

    rom[0] = 1;

    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window *window = SDL_CreateWindow(
        "ASH-8 Monitor",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        COLS * SCALE,
        ROWS * SCALE,
        0
    );

    SDL_Renderer *renderer = SDL_CreateRenderer(
        window,
        -1,
        SDL_RENDERER_ACCELERATED
    );

    CPU cpu = {0};

    bool running = true;
    SDL_Event e;

    while (running) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                running = false;
            }
        }

        // Clear screen memory
        for (int i = 0; i < COLS * ROWS; i++) {
            screen[i] = 0;
        }

        // Write into screen memory
        for (int i = 0; i < 20; i++) {
            screen[500 + cpu.x + i] = 'A';
        }

       uint8_t instruction = rom[cpu.pc];

       if (instruction == 1){
       	cpu.x++;
       }

       cpu.pc++;

        if (cpu.x > 20) {
            cpu.x = 0;
        }

        // Monitor reads screen memory
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
        SDL_Delay(100);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
