#include "core.h"
#include "graphics.h"

bool initialize_graphics(graphics* gpu) {
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    printf("SDL couldn't initialize. error: %s\n", SDL_GetError());
    return false;
  }
  if ((gpu->gWindow = SDL_CreateWindow("Chip-8", 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_ALLOW_HIGHDPI)) == NULL) {
    printf("SDL couldn't create a window. error: %s\n", SDL_GetError());
    return false;
  }

  if ((gpu->gRenderer = SDL_CreateRenderer(gpu->gWindow, -1, SDL_RENDERER_ACCELERATED)) == NULL) {
    printf("SDL couldn't create a renderer in the window. error: %s\n", SDL_GetError());
    return false;
  }

  if (!!SDL_RenderSetLogicalSize(gpu->gRenderer, CHIP8_WIDTH, CHIP8_HEIGHT)) {
    printf("SDL couldn't set the CHIP-8 resolution in the renderer. error: %s\n", SDL_GetError());
    return false;
  }

  if (!!SDL_SetRenderDrawColor(gpu->gRenderer, 255, 255, 255, 255)) {
    printf("SDL couldn't initialize the renderer's default color to white. error: %s\n", SDL_GetError());
    return false;
  }
  SDL_RenderClear(gpu->gRenderer);
  return true;
}

void clear_graphics(graphics* gpu) {
  memset(gpu->render_buffer, 0x0, CHIP8_WIDTH * CHIP8_HEIGHT);
}

// TRASH drawing code. so slow.
// TODO - replace this with code that blits a surface
// see: http://hg.libsdl.org/SDL/file/e12c38730512/test/testsprite2.c for example
void draw_pixel(graphics* gpu, uint8_t x, uint8_t y) {
  SDL_SetRenderDrawColor(gpu->gRenderer, 0, 0, 0, 255);
  SDL_Rect r;
  r.x = x;
  r.y = y;
  r.w = 1;
  r.h = 1;
  SDL_RenderFillRect(gpu->gRenderer, &r);
}

void render_graphics(graphics* gpu) {
  SDL_SetRenderDrawColor(gpu->gRenderer, 255, 255, 255, 255);
  SDL_RenderClear(gpu->gRenderer);
  
  // draw_pixels
  for (int y = 0; y < CHIP8_HEIGHT; y++) {
    for (int x = 0; x < CHIP8_WIDTH; x++) {
      if (gpu->render_buffer[y][x] & 0x1) {
	draw_pixel(gpu, x, y);
      }
    } 
  }

  SDL_RenderPresent(gpu->gRenderer);
}

void destroy_graphics(graphics* gpu) {
  if (gpu->gRenderer != NULL) {
    SDL_DestroyRenderer(gpu->gRenderer);
    gpu->gRenderer = NULL;
  }

  if (gpu->gWindow != NULL) {
    SDL_DestroyWindow(gpu->gWindow);
    gpu->gWindow = NULL;
  }

  SDL_Quit();
}

void load_sprites_into_pointer(ubyte* p) {
  // Copy sprites in, two at a time.
  memcpy(p, "\xF0\x90\x90\x90\xF0\x20\x60\x20\x20\x70", 2 * HEX_SPRITE_BYTES_ON_DISK);
  memcpy((p + 2 * HEX_SPRITE_BYTES_ON_DISK), "\xF0\x10\xF0\x80\xF0\xF0\x10\xF0\x10\xF0", 2 * HEX_SPRITE_BYTES_ON_DISK);
  memcpy((p + 4 * HEX_SPRITE_BYTES_ON_DISK), "\x90\x90\xF0\x10\x10\xF0\x80\xF0\x10\xF0", 2 * HEX_SPRITE_BYTES_ON_DISK);
  memcpy((p + 6 * HEX_SPRITE_BYTES_ON_DISK), "\xF0\x80\xF0\x90\xF0\xF0\x10\x20\x40\x40", 2 * HEX_SPRITE_BYTES_ON_DISK);
  memcpy((p + 8 * HEX_SPRITE_BYTES_ON_DISK), "\xF0\x90\xF0\x90\xF0\xF0\x90\xF0\x10\xF0", 2 * HEX_SPRITE_BYTES_ON_DISK);
  memcpy((p + 0xA * HEX_SPRITE_BYTES_ON_DISK), "\xF0\x90\xF0\x90\x90\xE0\x90\xE0\x90\xE0", 2 * HEX_SPRITE_BYTES_ON_DISK);
  memcpy((p + 0xC * HEX_SPRITE_BYTES_ON_DISK), "\xF0\x80\x80\x80\xF0\xE0\x90\x90\x90\xE0", 2 * HEX_SPRITE_BYTES_ON_DISK);
  memcpy((p + 0xE * HEX_SPRITE_BYTES_ON_DISK), "\xF0\x80\xF0\x80\xF0\xF0\x80\xF0\x80\x80", 2 * HEX_SPRITE_BYTES_ON_DISK);
}

int place_sprite(core* cpu, graphics* gpu, uint8_t sprite_length, reg start_x, reg start_y) {
  int collision = 0;

  for (int n = 0; n < sprite_length; n++) {
    for (int w = 0; w < SPRITE_WIDTH; w++) {
      ubyte sprite_byte = cpu->ram[cpu->I + n];
      ubyte sprite_pixel = (sprite_byte >> ((SPRITE_WIDTH - 1) - w)) & 0x1;
      int x = (start_x + w) % CHIP8_WIDTH;
      int y = (start_y + n) % CHIP8_HEIGHT;
      if (sprite_pixel && gpu->render_buffer[y][x]) {
	collision = 1;
      }
      gpu->render_buffer[y][x] ^= sprite_pixel;
    }
  }
  return collision;
}
