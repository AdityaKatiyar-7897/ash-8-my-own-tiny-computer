#include <SDL.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#define COLS 40
#define ROWS 25
#define SCALE 16

typedef struct{
	uint8_t key;
} Keyboard;

typedef struct{
	int x;
	int y;
	int a;
	int addr;

	bool zero;
	
	int pc;
	bool halted;
} CPU;

void video_step(uint8_t screen[], CPU *cpu, uint8_t ram[])
{
	int length = ram[0];

	if (length < 1){
		length = 1;
	}

	if (length > 20){
		length = 20;
	}

	for (int i = 0; i < length; i++){
		screen[(12 + ram[2]) * COLS + ram[1] + i] = 'A';
	}
	}



void cpu_step(CPU *cpu, uint8_t rom[] , uint8_t ram[], Keyboard *keyboard)
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
    	ram[cpu->addr] = cpu->a;
    }

    if (instruction == 9){

        cpu->a = ram[cpu->addr];
    	
    }

    if(instruction == 10){
    	cpu ->addr++;
    }

    if (instruction == 12){
    	cpu->a = keyboard->key;
    }

    if (instruction == 13){
    	if (cpu->a == 'a'){
    		cpu->x--;
    	}

    	if (cpu->a == 'd'){
    		cpu->x++;
    	}

    	if (cpu->a == 'w'){
    		cpu->y--;
    	}

    	if (cpu->a == 's'){
    		cpu->y++;
    	}
    }

    if (instruction == 14){
    	if (cpu->zero){
    		cpu->pc = -1;
    	}
    }

    if (instruction == 15){
    	cpu->pc = rom[cpu->pc + 1] -1;
    }

    if (instruction == 16){
    	if (cpu->zero){
    		cpu->pc = rom[cpu->pc +1] -1;
    	}
    }

    if (instruction == 17){
    	cpu->zero = (cpu->a == 0);
    }

    if (instruction == 18){
    	ram[1] = cpu->x;
    }

    if (instruction == 19){
    	ram[2] = cpu->y;
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
    Keyboard keyboard = {0};

rom[0] = 1;   // X++
rom[1] = 18;  // RAM[1] = X

rom[2] = 15;  // JUMP
rom[3] = 0;   // back to start

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
    cpu.a = 5;

    ram[0] = 10;
    ram[1] = 5;
    ram[2] = 5;

    bool running = true;
    SDL_Event e;

    while (running) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                running = false;
            }

            if (e.type == SDL_KEYDOWN){
            	keyboard.key = e.key.keysym.sym;
            }

            if (e.type == SDL_KEYUP) {
            	keyboard.key = 0;
            }
        }

        // Clear screen memory
        for (int i = 0; i < COLS * ROWS; i++) {
            screen[i] = 0;
        }

        video_step(screen, &cpu, ram);
        

        cpu_step(&cpu, rom, ram, &keyboard);

        char title[128];

        snprintf(
        	title,
        	sizeof(title),
        	"ASH-8  KEY=%d  A=%d  ADDR=%d  RAM[ADDR]=%d  X=%d  Y=%d  PC=%d , ZERO=%d",
        	keyboard.key,
        	cpu.a,
        	cpu.addr,
        	ram[cpu.addr],
        	cpu.x,
        	cpu.y,
        	cpu.pc,
        	cpu.zero
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
