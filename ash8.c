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
    int ball_x = ram[1];
    int ball_y = ram[2];

    if (ball_x >= 0 && ball_x < COLS && ball_y >= 0 && ball_y < ROWS) {
        screen[ball_y * COLS + ball_x] = 'A';
    }

    int paddle_y = ram[4];

    for (int i = 0; i < 5; i++) {
        int y = paddle_y + i;

        if (y >= 0 && y < ROWS) {
            screen[y * COLS + 2] = 'A';
        }
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

    if(instruction == 11){
    	cpu->a = rom[cpu->pc + 1];
    	cpu->pc++;
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
            cpu->pc = rom[cpu->pc + 1] - 1;
        } else {
            cpu->pc++;
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

    if (instruction == 22){
    	cpu->zero = (cpu->a == ram[cpu->addr]);
    }

    if (instruction == 23){
        cpu->addr = rom[cpu->pc + 1];
        cpu->pc++;
    }

    if (instruction == 24){
    	cpu->a--;
    }

    if (instruction == 25){
    	cpu->halted = true;
    }

    if(instruction == 26){
        ram[cpu->addr]++;
    }

    if (instruction == 27){
    	ram[cpu->addr]--;
    }

    if (instruction == 28){ // instruction to comopare wiht the immediate
    	cpu->zero = (cpu->a == rom[cpu->pc + 1]);
    	cpu->pc++;
    }

    if (instruction == 29){
    	cpu->a++;
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

    cpu.addr = 1;

    ram[1] = 20;  // ball x
    ram[2] = 12;  // ball y
    
    ram[4] = 10;  // paddle y
    
    ram[7] = 0;   // ball dx: 0 left, 1 right
    ram[8] = 1;   // ball dy: 0 up,   1 down
    
    // INPUT
    rom[0] = 12;
    rom[1] = 28; rom[2] = 'w';
    rom[3] = 16; rom[4] = 245;
    rom[5] = 28; rom[6] = 's';
    rom[7] = 16; rom[8] = 250;
    rom[9] = 15; rom[10] = 30;
    
    // X DIRECTION
    rom[30] = 23; rom[31] = 7;
    rom[32] = 9;
    rom[33] = 28; rom[34] = 1;
    rom[35] = 16; rom[36] = 150;
    
    // MOVE LEFT
    rom[37] = 23; rom[38] = 1;
    rom[39] = 9;
    rom[40] = 28; rom[41] = 3;
    rom[42] = 16; rom[43] = 100;
    
    rom[44] = 23; rom[45] = 1;
    rom[46] = 9;
    rom[47] = 17;
    rom[48] = 16; rom[49] = 70;
    
    rom[50] = 23; rom[51] = 1;
    rom[52] = 27;
    rom[53] = 15; rom[54] = 190;
    
    // RESET IF MISSED
    rom[70] = 23; rom[71] = 1;
    rom[72] = 11; rom[73] = 20;
    rom[74] = 8;
    
    rom[75] = 23; rom[76] = 2;
    rom[77] = 11; rom[78] = 12;
    rom[79] = 8;
    
    rom[80] = 23; rom[81] = 7;
    rom[82] = 11; rom[83] = 1;
    rom[84] = 8;
    
    rom[85] = 15; rom[86] = 190;
    
    // PADDLE COLLISION CHECK
    rom[100] = 23; rom[101] = 4;
    rom[102] = 9;
    rom[103] = 23; rom[104] = 2;
    
    rom[105] = 22; rom[106] = 16; rom[107] = 130;
    rom[108] = 29; rom[109] = 22; rom[110] = 16; rom[111] = 130;
    rom[112] = 29; rom[113] = 22; rom[114] = 16; rom[115] = 130;
    rom[116] = 29; rom[117] = 22; rom[118] = 16; rom[119] = 130;
    rom[120] = 29; rom[121] = 22; rom[122] = 16; rom[123] = 130;
    
    rom[124] = 15; rom[125] = 44;
    
    // HIT PADDLE
    rom[130] = 23; rom[131] = 7;
    rom[132] = 11; rom[133] = 1;
    rom[134] = 8;
    rom[135] = 15; rom[136] = 190;
    
    // MOVE RIGHT
    rom[150] = 23; rom[151] = 1;
    rom[152] = 9;
    rom[153] = 29;
    rom[154] = 28; rom[155] = 39;
    rom[156] = 16; rom[157] = 170;
    
    rom[158] = 23; rom[159] = 1;
    rom[160] = 26;
    rom[161] = 15; rom[162] = 190;
    
    // HIT RIGHT WALL
    rom[170] = 23; rom[171] = 7;
    rom[172] = 11; rom[173] = 0;
    rom[174] = 8;
    rom[175] = 15; rom[176] = 190;
    
    // Y DIRECTION
    rom[190] = 23; rom[191] = 8;
    rom[192] = 9;
    rom[193] = 28; rom[194] = 1;
    rom[195] = 16; rom[196] = 220;
    
    // MOVE UP
    rom[197] = 23; rom[198] = 2;
    rom[199] = 9;
    rom[200] = 17;
    rom[201] = 16; rom[202] = 210;
    
    rom[203] = 23; rom[204] = 2;
    rom[205] = 27;
    rom[206] = 15; rom[207] = 0;
    
    // HIT TOP
    rom[210] = 23; rom[211] = 8;
    rom[212] = 11; rom[213] = 1;
    rom[214] = 8;
    rom[215] = 15; rom[216] = 0;
    
    // MOVE DOWN
    rom[220] = 23; rom[221] = 2;
    rom[222] = 9;
    rom[223] = 29;
    rom[224] = 28; rom[225] = 24;
    rom[226] = 16; rom[227] = 235;
    
    rom[228] = 23; rom[229] = 2;
    rom[230] = 26;
    rom[231] = 15; rom[232] = 0;
    
    // HIT BOTTOM
    rom[235] = 23; rom[236] = 8;
    rom[237] = 11; rom[238] = 0;
    rom[239] = 8;
    rom[240] = 15; rom[241] = 0;
    
    // PADDLE UP
    rom[245] = 23; rom[246] = 4;
    rom[247] = 27;
    rom[248] = 15; rom[249] = 30;
    
    // PADDLE DOWN
    rom[250] = 23; rom[251] = 4;
    rom[252] = 26;
    rom[253] = 15; rom[254] = 30;
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
        

        for (int i = 0; i < 40; i++) {
            cpu_step(&cpu, rom, ram, &keyboard);
        }

        char title[128];

        snprintf(
            title,
            sizeof(title),
            "BALL=(%d,%d) PADDLE=%d DX=%d DY=%d PC=%d",
            ram[1],
            ram[2],
            ram[4],
            ram[7],
            ram[8],
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
