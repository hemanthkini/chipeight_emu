#include <SDL2/SDL.h>
#include "structures.h"

#ifndef __INCLUDE_INPUT
#define __INCLUDE_INPUT

void initialize_input();
void process_input(input* keyboard, SDL_Event e);

#endif
