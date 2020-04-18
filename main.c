#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <SDL2/SDL.h>

#include "core.h"

int main() {
  printf("Initializing...\n");

  core* cpu = calloc(1, sizeof(core));
  initialize_core(cpu);
  printf("\"CPU\" initialized.\n");

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
