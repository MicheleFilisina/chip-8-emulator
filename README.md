# CHIP-8 Emulator

A simple CHIP-8 emulator written in C++, rendered with SDL2.

[![License: GPL v3](https://img.shields.io/badge/License-GPLv3-blue.svg)](LICENSE)

## Contents

- [Requirements](#requirements)
- [Build](#build)
- [Usage](#usage)
- [Controls](#controls)
- [Technical reference](#technical-reference)
  - [CPU](#cpu)
  - [Memory](#memory)
- [Acknowledgments](#acknowledgments)
- [License](#license)

## Requirements

- A C++17 compiler
- [SDL2](https://www.libsdl.org/) development headers

On Debian/Ubuntu:

```bash
sudo apt install libsdl2-dev
```

## Build

```bash
g++ -std=c++17 chip8.cpp -o chip8 $(pkg-config --cflags --libs sdl2)
```

## Usage

```bash
./chip8 <Scale> <Delay> <ROM>
```

| Argument | Description                                              |
| -------- | --------------------------------------------------------- |
| `Scale`  | Integer factor to scale the 64x32 display by   |
| `Delay`  | Delay in milliseconds between emulated cycles              |
| `ROM`    | Path to a CHIP-8 ROM program/game to load                          |

Example:

```bash
./chip8 10 3 Tetris.ch8
```

## Controls

The original CHIP-8 keypad is a 4x4 hex layout, mapped onto the keyboard in the same physical grid:

CHIP-8 keypad -> Keyboard

|--------------|---|---|---|---|---|---|---|---|---|
|       1       |  2  |  3  |  C  |  ->   |    1     |  2  |  3  |  4  |
|       4       |  5  |  6  |  D  |   ->  |    Q     |  W  |  E  |  R  |
|       7       |  8  |  9  |  E  |   ->  |    A     |  S  |  D  |  F  |
|       A       |  0  |  B  |  F  |   ->  |    Z     |  X  |  C  |  V  |

Press `Esc` to quit.

## Technical reference

The specifics for this project are taken from [Cowgod's Chip-8 Technical Reference v1.0](http://devernay.free.fr/hacks/chip8/C8TECH10.HTM#2.1).

### CPU

- 16 general-purpose 1-byte registers: `V0`-`VE`, plus `VF`, which is used as a flag to hold the result of certain operations.
- 1 register (`I`) to store a 2-byte memory address; only its lowest 12 bits are used.
- 1 16-bit Program Counter (`PC`). Since instructions are 2 bytes long, each fetch reads one byte from `PC` and one from `PC + 1`, then merges them together. Before executing the fetched instruction, `PC` is incremented by 2.
- A stack, represented as an array of 16 16-bit addresses, with an 8-bit stack pointer (`SP`) indexing into it.
- 2 8-bit timers (delay and sound). When non-zero, each is decremented by 1 at a rate of 60Hz.

### Memory

```
┌────────────────────────────────────────────────────┐
│                 CHIP-8 MEMORY MAP                  │
├────────────────────────────────────────────────────┤
│                                                    │
│  0x000  ───────────────────────────────────────────│
│  (0)    ╔═════════════════════════════════════════╗│
│         ║  Reserved for Interpreter               ║│
│         ║  (512 bytes)                            ║│
│  0x1FF  ╚═════════════════════════════════════════╝│
│                                                    │
│  0x200  ───────────────────────────────────────────│
│  (512)  ╔═════════════════════════════════════════╗│
│         ║  Program / Data Space                   ║│
│  0x5FF  ║  (1536 bytes)                           ║│
│         ╚═════════════════════════════════════════╝│
│                                                    │
│  0x600  ───────────────────────────────────────────│
│  (1536) ╔═════════════════════════════════════════╗│
│         ║  ETI 660 Program Space                  ║│
│  0xFFF  ║  (3584 bytes)                           ║│
│  (4095) ╚═════════════════════════════════════════╝│
│                                                    │
├────────────────────────────────────────────────────┤
│  Total RAM: 4,096 bytes (4 KB)                     │
└────────────────────────────────────────────────────┘
```

## Acknowledgments

- [Cowgod's Chip-8 Technical Reference v1.0](http://devernay.free.fr/hacks/chip8/C8TECH10.HTM#2.1)
- [Austin Morlan's "Building a CHIP-8 Emulator"](https://austinmorlan.com/posts/chip8_emulator/), used as a reference for the opcode dispatch table and SDL platform layer

## License

Distributed under the terms of the [GNU General Public License v3.0](LICENSE).
