#include <SDL2/SDL.h>
#include <stdbool.h>

typedef struct graphics {
  SDL_Window* gWindow;
  SDL_Surface* gScreenSurface;
} graphics;


#define CHIP8_WIDTH 64
#define CHIP8_HEIGHT 32

#define RESOLUTION_MULTIPLIER 10
#define SCREEN_HEIGHT CHIP8_HEIGHT * RESOLUTION_MULTIPLIER
#define SCREEN_WIDTH CHIP8_WIDTH * RESOLUTION_MULTIPLIER


bool initialize_graphics(graphics* gpu) {
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    printf("SDL couldn't initialize. error: %s\n", SDL_GetError());
    return false;
  }
  if ((gpu->gWindow = SDL_CreateWindow("Chip-8", 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_ALLOW_HIGHDPI)) == NULL) {
    printf("SDL couldn't create a window. error: %s\n", SDL_GetError());
    return false;
  }

  gpu->gScreenSurface = SDL_GetWindowSurface(gpu->gWindow);
  return true;
}

void render_graphics(graphics* gpu) {
  //TODO remove
  /*
  SDL_Surface* testImageSurface = SDL_LoadBMP("test_images/test.bmp");
  if (testImageSurface == NULL) {
    printf("Failed to load bitmap!\n");
    return;
  }
  SDL_BlitSurface(testImageSurface, NULL, gpu->gScreenSurface, NULL);
  */
  SDL_UpdateWindowSurface(gpu->gWindow);
}

void destroy_graphics(graphics* gpu) {
  SDL_FreeSurface(gpu->gScreenSurface);
  gpu->gScreenSurface = NULL;

  SDL_DestroyWindow(gpu->gWindow);
  gpu->gWindow = NULL;

  SDL_Quit();
}
