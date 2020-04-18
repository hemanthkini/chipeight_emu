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
  SDL_SetRenderDrawColor(gpu->gRenderer, 255, 255, 255, 255);
  SDL_RenderClear(gpu->gRenderer);
}

void render_graphics(graphics* gpu) {
  SDL_Rect r;
  r.x = 5;
  r.y = 5;
  r.w = 5;
  r.h = 5;
  SDL_SetRenderDrawColor(gpu->gRenderer, 255, 0, 0, 255);
  SDL_RenderFillRect(gpu->gRenderer, &r);
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
  
}
