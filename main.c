#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <SDL2/SDL.h>

#include "core.h"

int load_rom(core* cpu, char* rom_path) {
  FILE *rom_file = fopen(rom_path, "rb");
  if (!rom_file) {
    printf("Failed to open rom file.\n");
    return -1;
  }
  
  size_t bytes_read = fread(program_start_in_core(cpu), sizeof(ubyte), RAM_LENGTH_IN_BYTES - PROGRAM_START_OFFSET, rom_file);
  if (bytes_read <= 0) {
    printf("Failed to read from rom file.\n");
    return bytes_read;
  }

  fclose(rom_file);
  return bytes_read;
}

int main() {
  printf("Initializing...\n");

  core* cpu = calloc(1, sizeof(core));
  initialize_core(cpu);
  printf("\"CPU\" initialized.\n");

  size_t bytes_loaded = load_rom(cpu, "./roms/Random Number Test [Matthew Mikolay, 2010].ch8");
  printf("%lu bytes loaded from rom file.\n", bytes_loaded);

  graphics* gpu = calloc(1, sizeof(graphics));
  if (!initialize_graphics(gpu)) {
    free(cpu);
    free(gpu);
    return -1;
  }
  printf("\"GPU\" initialized.\n");

  bool quit = false;
  SDL_Event e;

  while (!quit) {
    // Consume all events waiting.
    while(SDL_PollEvent(&e) != 0) {
      if (e.type == SDL_QUIT) {
	quit = true;
      }
      // Handle keypress events here.
    }

    // Update core and gpu here.
    tick(cpu, gpu);
    render_graphics(gpu);
  }

  destroy_graphics(gpu);
  free(gpu);
  free(cpu);
}
