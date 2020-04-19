#include <SDL2/SDL.h>

#ifndef __INCLUDE_STRUCTURES
#define __INCLUDE_STRUCTURES

#define CHIP8_WIDTH 64
#define CHIP8_HEIGHT 32

typedef struct graphics {
  uint8_t render_buffer[CHIP8_HEIGHT][CHIP8_WIDTH];
  SDL_Window* gWindow;
  SDL_Renderer* gRenderer;
} graphics;

#define NUM_REGISTERS 16
#define STACK_LENGTH 16 
#define RAM_LENGTH_IN_BYTES 4096

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
  uint32_t last_delay_ticks;
  uint32_t last_sound_ticks;
} core;

#define NUM_KEYS 16

typedef struct input {
  uint8_t key_down[NUM_KEYS];
  uint8_t num_keys_pressed;
  uint8_t last_key_pressed_index;
} input;

#endif
