#include <SDL2/SDL.h>
#include <stdbool.h>
#include "structures.h"

#ifndef __INCLUDE_GRAPHICS
#define __INCLUDE_GRAPHICS

#define CHIP8_WIDTH 64
#define CHIP8_HEIGHT 32

#define RESOLUTION_MULTIPLIER 10
#define SCREEN_HEIGHT CHIP8_HEIGHT * RESOLUTION_MULTIPLIER
#define SCREEN_WIDTH CHIP8_WIDTH * RESOLUTION_MULTIPLIER

#define HEX_SPRITE_WIDTH 8
#define SPRITE_WIDTH HEX_SPRITE_WIDTH
#define HEX_SPRITE_HEIGHT 5
#define HEX_SPRITE_BYTES_ON_DISK HEX_SPRITE_HEIGHT

bool initialize_graphics(graphics* gpu);
void clear_graphics(graphics* gpu);
void render_graphics(graphics* gpu);
void destroy_graphics(graphics* gpu);
void load_sprites_into_pointer(uint8_t* p);
int place_sprite(core* cpu, graphics* gpu, uint8_t sprite_length, uint8_t x, uint8_t y);
#endif
