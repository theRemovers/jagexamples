#include <jagdefs.h>
#include <jagtypes.h>
#include <stdlib.h>

#include <interrupt.h>
#include <display.h>
#include <sprite.h>

#include <sound.h>

#include <joypad.h>

#include <console.h>

extern phrase image;

#define IMGW 320
#define IMGH 240

int main(int argc, char *argv[]) {
  TOMREGS->vmode = CRY16|CSYNC|BGEN|PWIDTH4|VIDEN;
  init_interrupts();
  init_display_driver();

  display *d = new_display(0);
  d->x = 16;
  d->y = 8;

  show_display(d);

  phrase *black = malloc(sizeof(phrase));
  memset(black, 0, sizeof(phrase));

  sprite *s = new_sprite(IMGW,IMGH,0,0,DEPTH16,&image);
  sprite *s2 = new_sprite(4,IMGH,0,0,DEPTH16,black);
  s->invisible = 1;

  s2->dwidth = 0;
  s2->trans = 0;

  attach_sprite_to_display_at_layer(s,d,1);
  attach_sprite_to_display_at_layer(s2,d,2);

  int w = 1;
  int h = 0;
  s->height = h;
  s->iwidth = (w*2+7)/8;
  s2->x = s->x + w;

  s->invisible = 0;

  while(1) {
    vsync();
    if(h < IMGH) {
      h++;
      s->height = h;
    }
    if(w < IMGW) {
      w++;
      s->iwidth = (w*2+7)/8;
      s2->x = s->x + w;
      if(w == IMGW) {
	detach_sprite_from_display(s2);
	free(black);
	free(s2);
      }
    }
  }

  return 0;
}

