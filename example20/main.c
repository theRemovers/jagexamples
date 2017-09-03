#include <jagdefs.h>
#include <jagtypes.h>

#include <stdlib.h>

#include <interrupt.h>
#include <display.h>
#include <sprite.h>

#include <screen.h>

#define NCOLS 76

extern uint16_t egyptPal[NCOLS];
extern uint8_t egyptTiles[];

typedef struct {
  uint16_t ncols;
  uint16_t nrows;
  uint8_t data[];
} level_t;

extern level_t egyptLevel;

#define WIDTH 320
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

int main(int argc, char *argv[]) {
  TOMREGS->vmode = RGB16|CSYNC|BGEN|PWIDTH4|VIDEN;
  init_interrupts();
  init_display_driver();

  display *d = new_display(0);

  //  d->x = 16;

  memcpy((void*)TOMREGS->clut1, egyptPal, NCOLS*sizeof(uint16_t));

  FILE *console = open_console(d, 0, HEIGHT, NCOLS / 2 + 1);

  screen *tiles = new_screen();
  set_simple_screen(DEPTH8, WIDTH, HEIGHT, tiles, (phrase *)egyptTiles);

  screen *screen1 = new_screen();
  screen *screen2 = new_screen();

  phrase *screen1_data = alloc_simple_screen(DEPTH8, WIDTH, HEIGHT, screen1);
  phrase *screen2_data = alloc_simple_screen(DEPTH8, WIDTH, HEIGHT, screen2);

  sprite *screen1_sprite = sprite_of_screen(0, 0, screen1);
  sprite *screen2_sprite = sprite_of_screen(WIDTH, 0, screen2);
  screen1_sprite->trans = 0;
  screen2_sprite->trans = 0;

  clear_screen(screen1);
  clear_screen(screen2);

  attach_sprite_to_display_at_layer(screen1_sprite, d, 0);
  attach_sprite_to_display_at_layer(screen2_sprite, d, 0);

  {
    int i;
    screen1->x = 0;
    for(i = 0; i < WIDTH/TILES_W; i++) {
      screen1->y = 0;
      copy_column(tiles, screen1, &egyptLevel, 0, HEIGHT/TILES_H, i);
      screen1->x += TILES_W;
    }
  }

  show_display(d);

  fprintf(console, "nrows = %d, ncols = %d\n", egyptLevel.nrows, egyptLevel.ncols);
  fprintf(console, "width = %d, height = %d\n", video_width / 4, video_height);

  while(1) {
    vsync();
  }
}
