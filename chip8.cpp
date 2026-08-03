#include <cstdint>
#include <cstring>
#include <chrono>
#include <random>
#include <fstream>

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

class Chip8{

	Chip8() : randGen(std::chrono::system_clock::now().time_since_epoch().count()){
		 randByte = std::uniform_int_distribution<uint8_t>(0, 255U);
	}
	
	public:
		uint8_t registers[16]{};
		uint8_t memory[4096]{};
		uint16_t index{};
		uint16_t pc{};
		uint16_t stack[16]{};
		uint8_t sp{};
		uint8_t delayTimer{};
		uint8_t soundTimer{};
		uint8_t keypad[16]{};
		uint8_t video[video_width * video_height]{};
		uint16_t opcode{};
		void LoadROM(char const* filename);
		void FDE();

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

	private:
		std::default_random_engine randGen;
		std::uniform_int_distribution<uint8_t> randByte;
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


Chip8::Chip8(){
	pc = start_address;

	//load fontset into memory
	for( int i = 0; i<n_char; i++){
		for (int j=0; j<char_weight; j++){
			memory[fontset_start_add + i*char_weight+j] = fontset[i][j];
		}
	}
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

	for(int i = xPos;i < n; i++){
		uint8_t spriteRow = memory[index + i];
		for(int j = yPos; j < 8; j++){
			uint8_t spritePixel = spriteRow & (0x80 >> j);
			uint8_t * screenPixel = &video[(yPos + i) * video_width + j + xPos];

			if (spritePixel){
				if (*screenPixel == 0xFF) registers[15] = 1;

				*screenPixel ^= 0xFF;
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
