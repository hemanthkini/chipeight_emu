/* "CPU" emulation of a standard CHIP-8 VM.
 *
 * Derived from http://devernay.free.fr/hacks/chip8/C8TECH10.HTM
 */

#include <stdlib.h>
#include "graphics.h"

#ifndef __INCLUDE_CORE
#define __INCLUDE_CORE

#define NUM_REGISTERS 16
#define STACK_LENGTH 16 
#define RAM_LENGTH_IN_BYTES 4096
#define PROGRAM_START_OFFSET 512 // Programs are loaded at this point in RAM
#define HEX_SPRITE_START_OFFSET 0x10 // Sprites start at this point in RAM

typedef uint8_t reg;
typedef uint8_t ubyte;
typedef uint16_t instruction;
typedef uint16_t ireg;

typedef struct core {
  ubyte ram[RAM_LENGTH_IN_BYTES];
  reg V[NUM_REGISTERS];
  ireg I; // generally holds memory addresses
  reg DT; // Delay timer
  reg ST; // Sound timer
  ireg PC; // currently holding address (program counter)
  ireg stack[STACK_LENGTH];
  reg SP; // stack pointer - points to the topmost level of the stack
} core;

// More Significant Nibble and Less Significant Nibble
#define more_significant_nibble_of(u) (((u) >> 4) & 0xF)
#define less_significant_nibble_of(u) ((u) & 0xF)

ubyte* program_start_in_core(core* c);
void initialize_core(core* cpu);
void tick(core* cpu, graphics* gpu);

#endif
