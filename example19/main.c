#include <jagdefs.h>
#include <jagtypes.h>
#include <stdlib.h>

#include <interrupt.h>
#include <display.h>
#include <sprite.h>

#include <screen.h>

#include <joypad.h>

#include <blit.h>

#define NCOLS 42

extern uint16_t willowPal[NCOLS];
extern phrase willowMap;

#define WIDTH 192
#define HEIGHT 180

int main(int argc, char *argv[]) {
  TOMREGS->vmode = RGB16|CSYNC|BGEN|PWIDTH4|VIDEN;
  init_interrupts();
  init_display_driver();

  phrase *map = &willowMap;
  uint16_t *pal = willowPal;

  display *d = new_display(0);
  d->x = 16;
  d->y = 8;

  show_display(d);

  screen *original = new_screen();
  set_simple_screen(DEPTH8, WIDTH, HEIGHT, original, map);

  memcpy(TOMREGS->clut1, pal, NCOLS*sizeof(uint16_t));

  joypad_state *j_state = malloc(sizeof(joypad_state));

  sprite *s1 = sprite_of_screen(0,0,original);

  attach_sprite_to_display_at_layer(s1,d,1);

  long old_joy = 0;
  long joy = 0;

  while(1) {
    vsync();
  }

  return 0;
}
