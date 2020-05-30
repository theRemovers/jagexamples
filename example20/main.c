#include <jagdefs.h>
#include <jagtypes.h>

#include <stdlib.h>

#include <interrupt.h>
#include <display.h>
#include <sprite.h>

#include <screen.h>

#include <joypad.h>

#define NCOLS 76

extern uint16_t egyptPal[NCOLS];
extern uint8_t egyptTiles[];

typedef struct {
  uint16_t ncols;
  uint16_t nrows;
  uint8_t data[];
} level_t;

extern level_t egyptLevel;

extern phrase black;

#define WIDTH 256
#define HEIGHT 192

#define TILES_W 16
#define TILES_H 16

#define TILES_PER_LINE (320 / TILES_W)

#include <console.h>

#define SWAP(typ, v1, v2) { typ tmp = v1; v1 = v2; v2 = tmp; }

void copy_column(screen *tiles, screen *tgt, level_t *lvl, int lrow, int hrow, int colno) {
  uint16_t nrows = lvl->nrows;
  uint8_t *data = lvl->data + colno * nrows;
  int i;
  for(i = lrow; i < hrow; i++) {
    int tileno = data[i];
    tiles->x = (tileno % TILES_PER_LINE) * TILES_W;
    tiles->y = (tileno / TILES_PER_LINE) * TILES_H;
    screen_copy_straight(tiles, tgt, TILES_W, TILES_H, MODE_S);
    tgt->y += TILES_H;
  }
}

void copy_zone(screen *tiles, screen *tgt, int x, int y, int lrow, int hrow, int lcol, int hcol) {
  int i;
  tgt->x = x;
  for(i = lcol; i < hcol; i++) {
    tgt->y = y;
    copy_column(tiles, tgt, &egyptLevel, lrow, hrow, i);
    tgt->x += TILES_W;
  }
}

int main(int argc, char *argv[]) {
  TOMREGS->vmode = RGB16|CSYNC|BGEN|PWIDTH4|VIDEN;
  init_interrupts();
  init_display_driver();

  display *d = new_display(0);

  d->x = 64;
  d->y = 16;

  memcpy((void*)TOMREGS->clut1, egyptPal, NCOLS*sizeof(uint16_t));

  FILE *console = open_console(d, 0, HEIGHT, NCOLS / 2 + 1);

  screen *tiles = new_screen();
  set_simple_screen(DEPTH8, 320, HEIGHT, tiles, (phrase *)egyptTiles);

  screen *screen1 = new_screen();
  screen *screen2 = new_screen();

  phrase *screen1_data = alloc_simple_screen(DEPTH8, WIDTH, HEIGHT, screen1);
  phrase *screen2_data = alloc_simple_screen(DEPTH8, WIDTH, HEIGHT, screen2);

  sprite *screen1_sprite = sprite_of_screen(0, 0, screen1);
  sprite *screen2_sprite = sprite_of_screen(WIDTH, 0, screen2);
  screen1_sprite->trans = 0;
  screen2_sprite->trans = 0;

  sprite *black_border = new_sprite(16, 1, -16, 0, DEPTH16, &black);
  black_border->trans = 0;
  black_border->dwidth = 0;
  black_border->height = HEIGHT;

  clear_screen(screen1);
  clear_screen(screen2);

  attach_sprite_to_display_at_layer(screen1_sprite, d, 0);
  attach_sprite_to_display_at_layer(screen2_sprite, d, 0);
  attach_sprite_to_display_at_layer(black_border, d, 1);

  copy_zone(tiles, screen1, 0, 0, 0, HEIGHT/TILES_H, 0, WIDTH/TILES_W);

  show_display(d);

  fprintf(console, "nrows = %d, ncols = %d\n", egyptLevel.nrows, egyptLevel.ncols);
  fprintf(console, "width = %d, height = %d\n", video_width / 4, video_height);

  joypad_state joypads;

  while(1) {
    vsync();
    read_joypad_state(&joypads);
    unsigned long cmd = joypads.j1;

    if(cmd & JOYPAD_RIGHT) {
      screen1_sprite->x--;
      screen2_sprite->x--;
    } else if(cmd & JOYPAD_LEFT) {
      screen1_sprite->x++;
      screen2_sprite->x++;
    }
  }
}
