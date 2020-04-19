/* "CPU" emulation of a standard CHIP-8 VM.
 *
 * Derived from http://devernay.free.fr/hacks/chip8/C8TECH10.HTM
 */

#include <stdlib.h>
#include "structures.h"
#include "graphics.h"

#ifndef __INCLUDE_CORE
#define __INCLUDE_CORE

#define PROGRAM_START_OFFSET 512 // Programs are loaded at this point in RAM
#define HEX_SPRITE_START_OFFSET 0x10 // Sprites start at this point in RAM

#define TIMER_RATE 60 // Timer rate in HZ
#define TIMER_TICK_IN_MS (1000 / TIMER_RATE)

// More Significant Nibble and Less Significant Nibble
#define more_significant_nibble_of(u) (((u) >> 4) & 0xF)
#define less_significant_nibble_of(u) ((u) & 0xF)

ubyte* program_start_in_core(core* c);
void initialize_core(core* cpu);
void tick(core* cpu, graphics* gpu, input* keyboard);

#endif
