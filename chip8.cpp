#include <cstdint>
#include <cstring>
#include <fstream>

const uint16_t start_address = 0x200;
const uint16_t fontset_start_add = 0x50;
const uint16_t n_char = 16;
const uint16_t char_weight = 5;

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

class Chip8
{
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
	uint32_t video[64 * 32]{};
	uint16_t opcode{};
	Chip8();
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