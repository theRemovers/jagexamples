#include <jagdefs.h>
#include <jagtypes.h>
#include <stdlib.h>

#include <interrupt.h>
#include <sprite.h>
#include <joypad.h>
#include <screen.h>
#include <blit.h>

extern phrase ball_gfx;
extern phrase ball_gfx;

#define WAVELEN (16*512*2)

extern short int wave[WAVELEN];

#define NB_SPRITES 416

#define COEF (2*12)

sprite *balls[NB_SPRITES];
int idxspr[NB_SPRITES];

int main(int argc, char *argv[]) {
  TOMREGS->vmode = RGB16|CSYNC|BGEN|PWIDTH4|VIDEN;
  init_interrupts();

  init_display_driver();

  display *d = new_display(0);

  int i = 0, idx = 0;

  for(i = 0; i < NB_SPRITES; i++) {
    balls[i] = new_sprite(16,16,0,0,DEPTH16,&ball_gfx);

    idx += COEF;
    idx &= WAVELEN-1;

    balls[i]->x = wave[idx+1]; 
    balls[i]->y = wave[idx]; 
  }

  for(i = NB_SPRITES-1; i >= 0; i--) {
    attach_sprite_to_display_at_layer(balls[i],d,2);
  }

  d->x = 8;
  d->y = 8;

  show_display(d);

  for(;;) {
    vsync();

    for(i = 0; i < NB_SPRITES-1; i++) {
      balls[i]->x = balls[i+1]->x;
      balls[i]->y = balls[i+1]->y;
    }
    idx += COEF;
    idx &= WAVELEN-1;

    balls[i]->x = wave[idx+1]; 
    balls[i]->y = wave[idx]; 
  }    

  return 0;
}

