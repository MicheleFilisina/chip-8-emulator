#include <cstdint>
#include <cstring>
#include <chrono>
#include <random>
#include <iostream>
#include <fstream>
#include <SDL2/SDL.h>

const uint16_t start_address = 0x200;
const uint16_t fontset_start_add = 0x50;
const uint16_t n_char = 16;
const uint16_t char_weight = 5;
const uint8_t video_width = 64;
const uint8_t video_height = 32;

uint8_t fontset[n_char][char_weight] = {
	{0xF0, 0x90, 0x90, 0x90, 0xF0}, // 0
	{0x20, 0x60, 0x20, 0x20, 0x70}, // 1
	{0xF0, 0x10, 0xF0, 0x80, 0xF0}, // 2
	{0xF0, 0x10, 0xF0, 0x10, 0xF0}, // 3
	{0x90, 0x90, 0xF0, 0x10, 0x10}, // 4
	{0xF0, 0x80, 0xF0, 0x10, 0xF0}, // 5
	{0xF0, 0x80, 0xF0, 0x90, 0xF0}, // 6
	{0xF0, 0x10, 0x20, 0x40, 0x40}, // 7
	{0xF0, 0x90, 0xF0, 0x90, 0xF0}, // 8
	{0xF0, 0x90, 0xF0, 0x10, 0xF0}, // 9
	{0xF0, 0x90, 0xF0, 0x90, 0x90}, // A
	{0xE0, 0x90, 0xE0, 0x90, 0xE0}, // B
	{0xF0, 0x80, 0x80, 0x80, 0xF0}, // C
	{0xE0, 0x90, 0x90, 0x90, 0xE0}, // D
	{0xF0, 0x80, 0xF0, 0x80, 0xF0}, // E
	{0xF0, 0x80, 0xF0, 0x80, 0x80} // F
};

class Platform{
	public:
		Platform(char const* title, int windowWidth, int windowHeight, int textureWidth, int textureHeight){
			SDL_Init(SDL_INIT_VIDEO);
			window = SDL_CreateWindow(title, 0, 0, windowWidth, windowHeight, SDL_WINDOW_SHOWN);
			renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
			texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, textureWidth, textureHeight);
		}

		~Platform(){
			SDL_DestroyTexture(texture);
			SDL_DestroyRenderer(renderer);
			SDL_DestroyWindow(window);
			SDL_Quit();
		}

		void Update(void const* buffer, int pitch){
			SDL_UpdateTexture(texture, nullptr, buffer, pitch);
			SDL_RenderClear(renderer);
			SDL_RenderCopy(renderer, texture, nullptr, nullptr);
			SDL_RenderPresent(renderer);
		}

		bool ProcessInput(uint8_t* keys){
			bool quit = false;
			SDL_Event event;

			while (SDL_PollEvent(&event)){
				switch (event.type){
					case SDL_QUIT:
					{
						quit = true;
					} break;

					case SDL_KEYDOWN:
					{
						switch (event.key.keysym.sym){
							case SDLK_ESCAPE:
							{
								quit = true;
							} break;

							case SDLK_x:
							{
								keys[0] = 1;
							} break;

							case SDLK_1:
							{
								keys[1] = 1;
							} break;

							case SDLK_2:
							{
								keys[2] = 1;
							} break;

							case SDLK_3:
							{
								keys[3] = 1;
							} break;

							case SDLK_q:
							{
								keys[4] = 1;
							} break;

							case SDLK_w:
							{
								keys[5] = 1;
							} break;

							case SDLK_e:
							{
								keys[6] = 1;
							} break;

							case SDLK_a:
							{
								keys[7] = 1;
							} break;

							case SDLK_s:
							{
								keys[8] = 1;
							} break;

							case SDLK_d:
							{
								keys[9] = 1;
							} break;

							case SDLK_z:
							{
								keys[0xA] = 1;
							} break;

							case SDLK_c:
							{
								keys[0xB] = 1;
							} break;

							case SDLK_4:
							{
								keys[0xC] = 1;
							} break;

							case SDLK_r:
							{
								keys[0xD] = 1;
							} break;

							case SDLK_f:
							{
								keys[0xE] = 1;
							} break;

							case SDLK_v:
							{
								keys[0xF] = 1;
							} break;
						}
					} break;

					case SDL_KEYUP:
					{
						switch (event.key.keysym.sym){
							case SDLK_x:
							{
								keys[0] = 0;
							} break;

							case SDLK_1:
							{
								keys[1] = 0;
							} break;

							case SDLK_2:
							{
								keys[2] = 0;
							} break;

							case SDLK_3:
							{
								keys[3] = 0;
							} break;

							case SDLK_q:
							{
								keys[4] = 0;
							} break;

							case SDLK_w:
							{
								keys[5] = 0;
							} break;

							case SDLK_e:
							{
								keys[6] = 0;
							} break;

							case SDLK_a:
							{
								keys[7] = 0;
							} break;

							case SDLK_s:
							{
								keys[8] = 0;
							} break;

							case SDLK_d:
							{
								keys[9] = 0;
							} break;

							case SDLK_z:
							{
								keys[0xA] = 0;
							} break;

							case SDLK_c:
							{
								keys[0xB] = 0;
							} break;

							case SDLK_4:
							{
								keys[0xC] = 0;
							} break;

							case SDLK_r:
							{
								keys[0xD] = 0;
							} break;

							case SDLK_f:
							{
								keys[0xE] = 0;
							} break;

							case SDLK_v:
							{
								keys[0xF] = 0;
							} break;
						}
					} break;
				}
			}
			return quit;
		}

	private:
		SDL_Window* window{};
		SDL_Renderer* renderer{};
		SDL_Texture* texture{};
};

class Chip8{

	public:
		Chip8();
		uint8_t registers[16]{};
		uint8_t memory[4096]{};
		uint16_t index{};
		uint16_t pc{};
		uint16_t stack[16]{};
		uint8_t sp{};
		uint8_t delayTimer{};
		uint8_t soundTimer{};
		uint8_t keypad[16]{};
		uint32_t video[video_width * video_height]{}; //it is 4 bytes cause SDL_PIXELFORMAT_RGBA8888 is used
		uint16_t opcode{};
		void LoadROM(char const* filename);
		void FDE();

		void Cycle();
		void OP_00E0();
		void OP_00EE();
		void OP_1nnn();
		void OP_2nnn();
		void OP_3xkk();
		void OP_4xkk();
		void OP_5xy0();
		void OP_6xkk();
		void OP_7xkk();
		void OP_8xy0();
		void OP_8xy1();
		void OP_8xy2();
		void OP_8xy3();
		void OP_8xy4();
		void OP_8xy5();
		void OP_8xy6();
		void OP_8xy7();
		void OP_8xyE();
		void OP_9xy0();
		void OP_Annn();
		void OP_Bnnn();
		void OP_Cxkk();
		void OP_Dxyn();
		void OP_Ex9E();
		void OP_ExA1();
		void OP_Fx07();
		void OP_Fx0A();
		void OP_Fx15();
		void OP_Fx18();
		void OP_Fx1E();
		void OP_Fx29();
		void OP_Fx33();
		void OP_Fx55();
		void OP_Fx65();
		void OP_NULL();

		void Table0();
		void Table8();
		void TableE();
		void TableF();

	private:
		std::default_random_engine randGen;
		std::uniform_int_distribution<uint8_t> randByte;

		typedef void (Chip8::*Chip8Func)();
		Chip8Func table[0xF + 1];
		Chip8Func table0[0xE + 1];
		Chip8Func table8[0xE + 1];
		Chip8Func tableE[0xE + 1];
		Chip8Func tableF[0x65 + 1];
};

void Chip8::FDE(){
	opcode = (memory[pc]<<8u) + memory[pc+1];
	pc+=2;
}


void Chip8::LoadROM(char const* filename)
{
	// Open the file as a stream of binary and move the file pointer to the end
	std::ifstream file(filename, std::ios::binary | std::ios::ate);

	if (file.is_open())
	{
		// Get size of file and allocate a buffer to hold the contents
		std::streampos size = file.tellg(); // end position = file size in bytes
		char* buffer = new char[size];

		// Go back to the beginning of the file and fill the buffer
		file.seekg(0, std::ios::beg);
		file.read(buffer, size);
		file.close();

		// Load the ROM contents into the Chip8's memory, starting at 0x200
		for (long i = 0; i < size; ++i)
		{
			memory[start_address + i] = buffer[i];
		}

		// Free the buffer
		delete[] buffer;
	}
}


Chip8::Chip8() : randGen(std::chrono::system_clock::now().time_since_epoch().count()){
	randByte = std::uniform_int_distribution<uint8_t>(0, 255U);

	pc = start_address;

	//load fontset into memory
	for( int i = 0; i<n_char; i++){
		for (int j=0; j<char_weight; j++){
			memory[fontset_start_add + i*char_weight+j] = fontset[i][j];
		}
	}

	// Set up function pointer table
	table[0x0] = &Chip8::Table0;
	table[0x1] = &Chip8::OP_1nnn;
	table[0x2] = &Chip8::OP_2nnn;
	table[0x3] = &Chip8::OP_3xkk;
	table[0x4] = &Chip8::OP_4xkk;
	table[0x5] = &Chip8::OP_5xy0;
	table[0x6] = &Chip8::OP_6xkk;
	table[0x7] = &Chip8::OP_7xkk;
	table[0x8] = &Chip8::Table8;
	table[0x9] = &Chip8::OP_9xy0;
	table[0xA] = &Chip8::OP_Annn;
	table[0xB] = &Chip8::OP_Bnnn;
	table[0xC] = &Chip8::OP_Cxkk;
	table[0xD] = &Chip8::OP_Dxyn;
	table[0xE] = &Chip8::TableE;
	table[0xF] = &Chip8::TableF;

	for (size_t i = 0; i <= 0xE; i++){
		table0[i] = &Chip8::OP_NULL;
		table8[i] = &Chip8::OP_NULL;
		tableE[i] = &Chip8::OP_NULL;
	}

	table0[0x0] = &Chip8::OP_00E0;
	table0[0xE] = &Chip8::OP_00EE;

	table8[0x0] = &Chip8::OP_8xy0;
	table8[0x1] = &Chip8::OP_8xy1;
	table8[0x2] = &Chip8::OP_8xy2;
	table8[0x3] = &Chip8::OP_8xy3;
	table8[0x4] = &Chip8::OP_8xy4;
	table8[0x5] = &Chip8::OP_8xy5;
	table8[0x6] = &Chip8::OP_8xy6;
	table8[0x7] = &Chip8::OP_8xy7;
	table8[0xE] = &Chip8::OP_8xyE;

	tableE[0x1] = &Chip8::OP_ExA1;
	tableE[0xE] = &Chip8::OP_Ex9E;

	for (size_t i = 0; i <= 0x65; i++){
		tableF[i] = &Chip8::OP_NULL;
	}

	tableF[0x07] = &Chip8::OP_Fx07;
	tableF[0x0A] = &Chip8::OP_Fx0A;
	tableF[0x15] = &Chip8::OP_Fx15;
	tableF[0x18] = &Chip8::OP_Fx18;
	tableF[0x1E] = &Chip8::OP_Fx1E;
	tableF[0x29] = &Chip8::OP_Fx29;
	tableF[0x33] = &Chip8::OP_Fx33;
	tableF[0x55] = &Chip8::OP_Fx55;
	tableF[0x65] = &Chip8::OP_Fx65;
}

void Chip8::Table0(){
	(this->*table0[opcode & 0x000Fu])();
}

void Chip8::Table8(){
	(this->*table8[opcode & 0x000Fu])();
}

void Chip8::TableE(){
	(this->*tableE[opcode & 0x000Fu])();
}

void Chip8::TableF(){
	(this->*tableF[opcode & 0x00FFu])();
}

void Chip8::OP_NULL(){}

void Chip8::Cycle(){
	// Fetch
	opcode = (memory[pc] << 8) | memory[pc + 1];

	// Increment the program counter before we execute
	pc += 2;

	// Decode and Execute
	(this->*table[(opcode & 0xF000u) >> 12u])();

	// Decrement delay and sound timer if not zero
	if (delayTimer > 0) --delayTimer;
	if (soundTimer > 0) --soundTimer;
}




/* ######### INSTRUCTIONS ######### */

// CLS: Clear The Display
void Chip8::OP_00E0(){
	//for(int i=0; i<64*32; i++) video[i] = 0;
	memset(video, 0, sizeof(video));
}

// RET: Return From a Subroutine
void Chip8::OP_00EE(){
	sp--;
	pc = stack[sp];
}

// JP Addr: jump to address nnn;
void Chip8::OP_1nnn(){
	uint16_t nnn = opcode & 0x0FFF;
	pc = nnn;
}

// CALL Addr: Call subroutine at nnn
void Chip8::OP_2nnn(){
	uint16_t nnn = opcode & 0x0FFF;
	stack[sp] = pc;
	sp++;
	pc = nnn;
}

// Skip next instrution if Vx = kk
void Chip8::OP_3xkk(){
	uint8_t x = (opcode>>8u) & 0x0F;
	uint8_t kk = opcode & 0x00FF;
	if(registers[x] == kk) pc += 2;
}

// Skip next instruction if Vx != kk
void Chip8::OP_4xkk(){
	uint8_t x = (opcode>>8u) & 0xF;
	uint8_t kk = opcode & 0xFF;
	if(registers[x] != kk) pc += 2;
}

// Skip next instruction if Vx = Vy
void Chip8::OP_5xy0(){
	uint8_t x = (opcode>>8u) & 0xF;
	uint8_t y = (opcode>>4u) & 0xF;
	if(registers[x] == registers[y]) pc += 2;
}

// Set Vx = kk
void Chip8::OP_6xkk(){
	uint8_t x = (opcode>>8u) & 0xF;
	uint8_t kk = opcode & 0xFF;
	registers[x] = kk;
}

// Set Vx = kk
void Chip8::OP_7xkk(){
	uint8_t x = (opcode>>8u) & 0xF;
	uint8_t kk = opcode & 0xFF;
	registers[x] += kk;
}

// Stores the value of register Vy in register Vx
void Chip8::OP_8xy0(){
	uint8_t x = (opcode>>8u) & 0xF;
	uint8_t y = (opcode>>4u) & 0xF;
	registers[x] = registers[y];
}

// Set Vx = Vx OR Vy
void Chip8::OP_8xy1(){
	uint8_t x = (opcode>>8u) & 0xF;
	uint8_t y = (opcode>>4u) & 0xF;
	registers[x] |= registers[y];
}

// Set Vx = Vx AND Vy
void Chip8::OP_8xy2(){
	uint8_t x = (opcode>>8u) & 0xF;
	uint8_t y = (opcode>>4u) & 0xF;
	registers[x] &= registers[y];
}

// Set Vx = Vx XOR Vy
void Chip8::OP_8xy3(){
	uint8_t x = (opcode>>8u) & 0xF;
	uint8_t y = (opcode>>4u) & 0xF;
	registers[x] ^= registers[y];
}

// Set Vx = Vx + Vy, set VF = carry
void Chip8::OP_8xy4(){
	uint8_t x = (opcode>>8u) & 0xF;
	uint8_t y = (opcode>>4u) & 0xF;

	uint16_t sum = registers[x] + registers[y];
	sum > 255 ? registers[15] = 1 : registers[15] = 0;

	registers[x] = sum;
}

// Set Vx = Vx - Vy, set VF = NOT borrow.
void Chip8::OP_8xy5(){
	uint8_t x = (opcode>>8u) & 0xF;
	uint8_t y = (opcode>>4u) & 0xF;

	registers[x] > registers[y] ? registers[15] = 1 : registers[15] = 0;

	registers[x] -= registers[y];
}

// Set Vx = Vx SHR 1
void Chip8::OP_8xy6(){
	uint8_t x = (opcode>>8) & 0xF;

	registers[15] = registers[x] & 0x1;
	registers[x] >>= 1;
}

// Set Vx = Vy - Vx, set VF = NOT borrow
void Chip8::OP_8xy7(){
	uint8_t x = (opcode>>8) & 0xF;
	uint8_t y = (opcode>>4) & 0xF;

	registers[y] > registers[x] ? registers[15] = 1 : registers[15] = 0;
	registers[x] = registers[y] - registers[x];
}

// Set Vx = Vx SHL 1
void Chip8::OP_8xyE(){
	uint8_t x = (opcode>>8) & 0xF;
	registers[15] = (registers[x] & 0x80) >> 7;
	registers[x] <<= 1;
}

// Skip next instruction if Vx != Vy
void Chip8::OP_9xy0(){
	uint8_t x = (opcode>>8) & 0xF;
	uint8_t y = (opcode>>4) & 0xF;
	if(registers[x] != registers[y]) pc += 2;
}

// Set I = nnn
void Chip8::OP_Annn(){
	uint16_t nnn = opcode & 0xFFF;
	index = nnn;
}

// Jump to location nnn + V0
void Chip8::OP_Bnnn(){
	uint16_t nnn = opcode & 0xFFF;
	pc = nnn + registers[0];
}

// Set Vx = random byte AND kk
void Chip8::OP_Cxkk(){
	uint8_t x = (opcode>>8) & 0xF;
	uint8_t kk = opcode & 0xFF;
	registers[x] = randByte(randGen) & kk;
}

// Display n-byte sprite starting at memory location I at (Vx, Vy), set VF = collision
void Chip8::OP_Dxyn(){
	uint8_t x = (opcode>>8) & 0xF;
	uint8_t y = (opcode>>4) & 0xF;
	uint8_t n = opcode & 0xF;

	uint8_t xPos = registers[x] % video_width;
	uint8_t yPos = registers[y] % video_height;

	registers[0xF] = 0;

	for(int i = 0;i < n; i++){
		uint8_t spriteRow = memory[index + i];
		for(int j = 0; j < 8; j++){
			uint8_t spritePixel = spriteRow & (0x80 >> j);
			uint32_t * screenPixel = &video[(yPos + i) * video_width + j + xPos];

			if (spritePixel){
				if (*screenPixel == 0xFFFFFFFF) registers[15] = 1;

				*screenPixel ^= 0xFFFFFFFF;
			}

		}
	}
}

// Skip next instruction if key with the value of Vx is pressed
void Chip8::OP_Ex9E(){
	uint8_t x = (opcode>>8) & 0x0F;

	uint8_t value = registers[x];

	if (keypad[value]) pc += 2;
}

// Skip next instruction if key with the value of Vx is not pressed
void Chip8::OP_ExA1(){
	uint8_t x = (opcode>>8) & 0x0F;

	uint8_t value = registers[x];

	if (!keypad[value]) pc += 2;
}

// Set Vx = delay timer value
void Chip8::OP_Fx07(){
	uint8_t x = (opcode>>8) & 0x0F;
	registers[x] = delayTimer;
}

//  Wait for a key press, store the value of the key in Vx.
void Chip8::OP_Fx0A(){
	uint8_t x = (opcode>>8) & 0x0F;

	bool keyPressed = false;
	for(int i=0; i<16; i++){
		if(keypad[i]){
			registers[x] = i;
			keyPressed = true;
			break;
		}
	}

	if(!keyPressed) pc -= 2;
}

// Set delay timer = Vx
void Chip8::OP_Fx15(){
	uint8_t x = (opcode>>8) & 0x0F;
	delayTimer = registers[x];
}

// Set sound timer = Vx
void Chip8::OP_Fx18(){
	uint8_t x = (opcode>>8) & 0x0F;
	soundTimer = registers[x];
}

// Set I = I + Vx
void Chip8::OP_Fx1E(){
	uint8_t x = (opcode>>8) & 0x0F;
	index += registers[x];
}

// Set I = location of sprite for digit Vx
void Chip8::OP_Fx29(){
	uint8_t x = (opcode>>8) & 0x0F;
	uint8_t value = registers[x];
	index = fontset_start_add + (value * char_weight);
}

// Store BCD representation of Vx in memory locations I, I+1, and I+2
void Chip8::OP_Fx33(){
	uint8_t x = (opcode>>8) & 0x0F;
	uint8_t value = registers[x];

	uint8_t ones = value % 10;
	value /= 10;

	uint8_t tens = value % 10;
	value /= 10;

	uint8_t hundreds = value % 10;

	memory[index] = hundreds;
	memory[index + 1] = tens;
	memory[index + 2] = ones;
}

// Store registers V0 through Vx in memory starting at location I
void Chip8::OP_Fx55(){
	uint8_t x = (opcode>>8) & 0x0F;
	for(int i=0; i<=x; i++){
		memory[index + i] = registers[i];
	}
}

// Read registers V0 through Vx from memory starting at location I
void Chip8::OP_Fx65(){
	uint8_t x = (opcode>>8) & 0x0F;
	for(int i=0; i<=x; i++){
		registers[i] = memory[index + i];
	}
}




int main(int argc, char * argv[])
{
	if (argc != 4){
		std::cerr << "Usage: " << argv[0] << " <Scale> <Delay> <ROM>\n";
		std::exit(EXIT_FAILURE);
	}

	int videoScale = std::stoi(argv[1]);
	int cycleDelay = std::stoi(argv[2]);
	char const* romFilename = argv[3];

	Platform platform("CHIP-8 Emulator", video_width * videoScale, video_height * videoScale, video_width, video_height);

	Chip8 chip8;
	chip8.LoadROM(romFilename);

	int videoPitch = sizeof(chip8.video[0]) * video_width;
	auto lastCycleTime = std::chrono::high_resolution_clock::now();
	bool quit = false;

	while (!quit){
		quit = platform.ProcessInput(chip8.keypad);
		auto currentTime = std::chrono::high_resolution_clock::now();
		float dt = std::chrono::duration<float, std::chrono::milliseconds::period>(currentTime - lastCycleTime).count();

		if (dt > cycleDelay){
			lastCycleTime = currentTime;
			chip8.Cycle();
			platform.Update(chip8.video, videoPitch);
		}
	}

	return 0;
}