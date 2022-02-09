#include <jagdefs.h>
#include <jagtypes.h>
#include <interrupt.h>
#include <screen.h>
#include <sprite.h>
#include "mydisplay.h"

#define WIDTH 320
#define HEIGHT 200

#define GFXIT 0
#define SCREEN1 0

int main(int argc, char *argv[]) {
  TOMREGS->vmode = RGB16|CSYNC|BGEN|PWIDTH4|VIDEN;

  VI = init_interrupts();

  screen *screen1 = new_screen();

  phrase *screen1_data = alloc_simple_screen(DEPTH8, WIDTH, HEIGHT, screen1);

  sprite *screen1_sprite = sprite_of_screen(0, 0, screen1);
  screen1_sprite->trans = 0;

  clear_screen(screen1);

  display_sprites[SCREEN1] = screen1_sprite;

  display_init();
  vblQueue[GFXIT] = display_refresh;

  TOMREGS->clut1[1] = RGBCOLOR(255, 0, 0);
  TOMREGS->clut1[2] = RGBCOLOR(0, 255, 0);
  TOMREGS->clut1[3] = RGBCOLOR(0, 0, 255);
  TOMREGS->clut1[4] = RGBCOLOR(0, 255, 255);

  {
    int x1 = WIDTH/2;
    int y1 = HEIGHT/2;
    int i;
    for(i = 0; i <= 50; i++) {
      line(screen1, x1, y1, x1+50, y1+i, 0x03030303);
    }
    for(i = 0; i <= 50; i++) {
      line(screen1, x1, y1, x1+i, y1+50, 0x04040404);
    }
    line(screen1, x1, y1, x1+50, y1+50, 0x02020202);
    for(i = 0; i <= 50; i++) {
      line(screen1, x1, y1, x1-50, y1+i, 0x03030303);
    }
    for(i = 0; i <= 50; i++) {
      line(screen1, x1, y1, x1-i, y1+50, 0x04040404);
    }
    line(screen1, x1, y1, x1-50, y1+50, 0x02020202);
    for(i = 0; i <= 50; i++) {
      line(screen1, x1, y1, x1-50, y1-i, 0x03030303);
    }
    for(i = 0; i <= 50; i++) {
      line(screen1, x1, y1, x1-i, y1-50, 0x04040404);
    }
    line(screen1, x1, y1, x1-50, y1-50, 0x02020202);
    for(i = 0; i <= 50; i++) {
      line(screen1, x1, y1, x1+50, y1-i, 0x03030303);
    }
    for(i = 0; i <= 50; i++) {
      line(screen1, x1, y1, x1+i, y1-50, 0x04040404);
    }
    line(screen1, x1, y1, x1+50, y1-50, 0x02020202);
    hline(screen1,x1-51,y1-51,x1+51,0X01010101);
    hline(screen1,x1-51,y1+51,x1+51,0X01010101);
    vline(screen1,x1-51,y1-51,y1+51,0x01010101);
    vline(screen1,x1+51,y1-51,y1+51,0x01010101);
  }

  TOMREGS->bg = 0x003f; // GREEN
  while(1) {
    vsync();
  }

  return 0;
}
