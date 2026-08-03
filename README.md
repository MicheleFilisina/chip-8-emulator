# chip-8-emulator



# specifics
the specifics for this project are taken from (http://devernay.free.fr/hacks/chip8/C8TECH10.HTM#2.1)[Cowgod's Chip-8 Technical Reference v1.0]

## CPU
16 general purpose registers of 1 byte each: V0, V1, V2...VE, VF.
VF it’s used as a flag to hold information about the result of operations.

1 register to store address of 2 byte. only its lowest 12 bits are used.

1 Program Counter register of 16 bits, since instutions are 2 byte long we fetch one byte prom PC and one byte from PC+1;
then we merge these values togheter. 
before executing the just fetched instruction we increment the PC by 2.

The stack is rappresented by an array of 16 16 bits address.
The stack pointer is a 8 bit index.

2 8 bits registers for delay and timers, when they are not 0 they are decremented by one at 60hz rate.

## Memory
+---------------+= 0xFFF (4095) End of Chip-8 RAM
|               |
|               |
|               |
|               |
|               |
| 0x200 to 0xFFF|
|     Chip-8    |
| Program / Data|
|     Space     |
|               |
|               |
|               |
+- - - - - - - -+= 0x600 (1536) Start of ETI 660 Chip-8 programs
|               |
|               |
|               |
+---------------+= 0x200 (512) Start of most Chip-8 programs
| 0x000 to 0x1FF|
| Reserved for  |
|  interpreter  |
+---------------+= 0x000 (0) Start of Chip-8 RAM
