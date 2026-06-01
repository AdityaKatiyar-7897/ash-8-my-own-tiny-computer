#include <SDL.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#define COLS 40
#define ROWS 25
#define SCALE 16

typedef struct{
	int x;
	int y;
	int a; //another new register (general purpose)
	int pc;
	bool halted;
} CPU;

void video_step(uint8_t screen[], CPU *cpu)
{
	int length = cpu->a;

	if (length < 1){
		length = 1;
	}

	if (length > 20){
		length = 20;
	}

	for (int i = 0; i < length; i++){
		screen[(12 + cpu->y) * COLS + cpu->x + i] = 'A';
	}
	}



void cpu_step(CPU *cpu, uint8_t rom[] , uint8_t ram[])
{
    if (cpu->halted) {
        return;
    }

    uint8_t instruction = rom[cpu->pc];

    if (instruction == 1) {
        cpu->a++;
        cpu->x++;
    }

    if (instruction == 2) {
        cpu->a--;
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

    if (instruction == 6) {
    	cpu->a++;
    }

    if (instruction == 7){
    	cpu->a--;
    }

    if (instruction == 8){
    	ram[0] = cpu->a;
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
    uint8_t ram[256] = {0};

   rom[0] = 6;
   rom[1] = 6;
   rom[2] = 6;
   rom[3] = 8;
   rom[4] = 3;

    

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

        video_step(screen, &cpu);
        

        cpu_step(&cpu, rom, ram);

        char title[128];

        snprintf(
        	title,
        	sizeof(title),
        	"ASH-8  A=%d  RAM0=%d  X=%d  Y=%d  PC=%d",
        	cpu.a,
        	ram[0],
        	cpu.x,
        	cpu.y,
        	cpu.pc
        );

        SDL_SetWindowTitle(window, title);

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
