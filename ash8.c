#include <SDL.h>
#include <stdint.h>
#include <stdbool.h>

#define COLS 40
#define ROWS 25
#define SCALE 16

typedef struct{
	int x;
	int y; //adding a new register to our cpu 
	int pc;
	bool halted;
} CPU;

void cpu_step(CPU *cpu, uint8_t rom[])
{
    if (cpu->halted) {
        return;
    }

    uint8_t instruction = rom[cpu->pc];

    if (instruction == 1) {
        cpu->x++;
    }

    if (instruction == 2) {
        cpu->x--;
    }

    if (instruction == 3) {
        cpu->pc = -1;
    }

    if (instruction == 4) {
        cpu->y++;
    }

    if (instruction == 5) {
        cpu->y--;
    }

    if (instruction == 0) {
        cpu->halted = true;
    }

    cpu->pc++;
}


int main(void)
{
    uint8_t screen[COLS * ROWS] = {0};
    uint8_t rom[256] = {0};

    for (int i = 0 ; i <10 ; i++){
    	rom[i] = 1;
    }

    for (int i = 10; i < 20; i++){
    	rom[i]  = 2; 
     }

     for (int i = 20; i < 30; i++){
     	rom[i] = 2;
     }

     for (int i = 30; i < 40; i++){
     	rom[i] = 5;
     }

    rom[40] = 3;

    

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
            screen[(12 + cpu.y) * COLS + cpu.x + i] = 'A';
        }

        cpu_step(&cpu, rom);

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
