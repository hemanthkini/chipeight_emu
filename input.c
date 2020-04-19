#include <SDL2/SDL.h>
#include "input.h"

void initialize_input() {

  return;
}

void process_input(input* keyboard, SDL_Event e) {
  uint8_t key_state = (e.type == SDL_KEYDOWN) ? 1 : 0;
  int key_index = -1;

  // handle numbers
  if (e.key.keysym.sym >= 0x30 && e.key.keysym.sym <= 0x39) {
    key_index = e.key.keysym.sym - 0x30;
  }
  
  // handle A-F
  if (e.key.keysym.sym >= 0x61 && e.key.keysym.sym <= 0x66) {
    key_index = 0xA + (e.key.keysym.sym - 0x61);
  }
  
  if (key_index != -1) {
    printf("Keyboard modified: %d\n", key_index);
    keyboard->key_down[key_index] = key_state;
    if (key_state) {
      keyboard->num_keys_pressed++;
      keyboard->last_key_pressed_index = key_index;
    } else {
      keyboard->num_keys_pressed--;
    }
  }
}
