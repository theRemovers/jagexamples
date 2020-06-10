#include <jagdefs.h>
#include <jagtypes.h>

#include <stdlib.h>

#include <interrupt.h>
#include <display.h>
#include <sprite.h>

#include <screen.h>

#include <joypad.h>

#include <console.h>

extern phrase sprite_gfx;

#define SPRITEW 64
#define SPRITEH 64

#define WIDTH 320
#define HEIGHT 200

int main(int argc, char *argv[]) {
  TOMREGS->vmode = RGB16|CSYNC|BGEN|PWIDTH4|VIDEN;
  init_interrupts();
  init_display_driver();

  display *d = new_display(0);

  d->x = 16;

  screen *sprite_scr = new_screen();
  set_simple_screen(DEPTH16,SPRITEW,SPRITEH,sprite_scr,&sprite_gfx);

  screen *fb = new_screen();
  alloc_simple_screen(DEPTH16,WIDTH,HEIGHT,fb);
  clear_screen(fb);

  sprite *fb_spr = sprite_of_screen(0, 0, fb);
  fb_spr->trans = 0;

  attach_sprite_to_display_at_layer(fb_spr, d, 0);

  show_display(d);

  sprite_scr->x = SPRITEW/2;
  sprite_scr->y = SPRITEH/2;
  fb->x = WIDTH/2;
  fb->y = HEIGHT/2;

  joypad_state joypads;

  int alpha = 0;
  while(1) {
    vsync();
    screen_rotate(sprite_scr, fb, alpha);
    read_joypad_state(&joypads);
    unsigned long cmd = joypads.j1;
    if(cmd & JOYPAD_RIGHT) {
      alpha++;
    } else if(cmd & JOYPAD_LEFT) {
      alpha--;
    }
  }
}
