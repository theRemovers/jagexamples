#include <jagdefs.h>
#include <jagtypes.h>
#include <stdlib.h>

#include <interrupt.h>
#include <display.h>
#include <sprite.h>
#include <collision.h>
#include <joypad.h>
#include <screen.h>
#include <blit.h>

extern phrase jagware_logo;
extern phrase sprite_gfx;
extern short int wave[4096];

#define NB_SCREENS 6
screen *screens[NB_SCREENS];

#define SPRITEW 64
#define SPRITEH 64

int main(int argc, char *argv[]) {
  TOMREGS->vmode = RGB16|CSYNC|BGEN|PWIDTH4|VIDEN;
  init_interrupts();
  init_display_driver();

  display *d = new_display(0);
  d->x = 16;
  d->y = 8;

  sprite *background = new_sprite(320,240,0,0,DEPTH16,&jagware_logo);
  attach_sprite_to_display_at_layer(background,d,0);

  screen *sprite_scr = new_screen();
  set_simple_screen(DEPTH16,SPRITEW,SPRITEH,sprite_scr,&sprite_gfx);

  int i;
  for(i = 0; i < NB_SCREENS; i++) {
    screens[i] = new_screen();
    alloc_simple_screen(DEPTH16,320,240,screens[i]);
  }
  sprite *fb_spr = sprite_of_screen(0,0,screens[0]);

  animation_chunk *anim = calloc(NB_SCREENS+1,sizeof(animation_chunk));
  for(i = 0; i < NB_SCREENS; i++) {
    anim[i].data = screens[i]->data;
    clear_screen(screens[i]);
    anim[i].speed = 1;

  }
  anim[NB_SCREENS].data = NULL;
  anim[NB_SCREENS].speed = 0;

  fb_spr->animation = anim;
  fb_spr->animation_data.index = 0;
  fb_spr->animation_data.counter = 1;
  fb_spr->animated = 1;

  attach_sprite_to_display_at_layer(fb_spr,d,1);

  show_display(d);

  int idx = 0;

  for(;;) {
    vsync();

    i = fb_spr->animation_data.index;
    i++;
    if(i >= NB_SCREENS) {
      i = 0;
    }
    screens[i]->y = wave[idx];
    screens[i]->x = wave[idx+1];
    screen_copy_straight(sprite_scr,screens[i],SPRITEW,SPRITEH,MODE_TRANSPARENT);
    idx += 2;
    idx &= 4095;
  }

  return 0;
}

