# chip-8-emulator



# specifics
the specifics for this project are taken from [Cowgod's Chip-8 Technical Reference v1.0](http://devernay.free.fr/hacks/chip8/C8TECH10.HTM#2.1)

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
┌─────────────────────────────────────────────────┐
│                  CHIP-8 MEMORY MAP              │
├─────────────────────────────────────────────────┤
│                                                 │
│  0x000 ─────────────────────────────────────    │
│  (0)    ╔═════════════════════════════════╗   │
│         ║  Reserved for Interpreter      ║   │
│         ║  (512 bytes)                   ║   │
│  0x1FF  ╚═════════════════════════════════╝   │
│                                                 │
│  0x200 ─────────────────────────────────────    │
│  (512)  ╔═════════════════════════════════╗   │
│         ║  Program / Data Space           ║   │
│         ║  (Most Chip-8 programs start    ║   │
│         ║   here)                         ║   │
│         ║                                 ║   │
│  0x5FF  ║  (1536 bytes)                   ║   │
│         ╚═════════════════════════════════╝   │
│                                                 │
│  0x600 ─────────────────────────────────────    │
│  (1536) ╔═════════════════════════════════╗   │
│         ║  ETI 660 Program Space          ║   │
│         ║  (Alternative start location)   ║   │
│         ║                                 ║   │
│  0xFFF  ║  (3584 bytes)                   ║   │
│  (4095) ╚═════════════════════════════════╝   │
│                                                 │
├─────────────────────────────────────────────────┤
│  Total RAM: 4,096 bytes (4 KB)                 │
└─────────────────────────────────────────────────┘
