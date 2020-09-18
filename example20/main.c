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

typedef struct {
  uint16_t grid_x;
  uint16_t grid_y;
} tile_t;

extern level_t egyptLevel;

extern phrase black;

#define WIDTH 320
#define HEIGHT 192

#define TILES_PER_LINE (320 / 16)
#define NUM_TILES (TILES_PER_LINE * 192 / 16)

#define DEBUG
#ifdef DEBUG
#include <console.h>
#endif

#define SWAP(typ, v1, v2) { typ tmp = v1; v1 = v2; v2 = tmp; }

tile_t *tile_data;
int map_x;
int map_w;

void init_map(void)
{
  int i;

  tile_data = (tile_t *) malloc(sizeof(tile_t) * NUM_TILES);

  for (i = 0; i < NUM_TILES; i++) {
    tile_data[i].grid_x = (i % TILES_PER_LINE) * 16;
    tile_data[i].grid_y = (i / TILES_PER_LINE) * 16;
  }

  map_x = 0;
  map_w = egyptLevel.ncols * 16;
}

void copy_column(screen *tiles, screen *tgt, level_t *lvl, int lrow, int hrow, int colno) {
  uint16_t nrows = lvl->nrows;
  uint8_t *data = lvl->data + colno * nrows;
  int i;
  for(i = lrow; i < hrow; i++) {
    int tileno = data[i];
    tile_t *tile = &tile_data[tileno];
    tiles->x = tile->grid_x;
    tiles->y = tile->grid_y;
    screen_copy_straight(tiles, tgt, 16, 16, MODE_S);
    tgt->y += 16;
  }
}

void copy_zone(screen *tiles, screen *tgt, level_t *lvl, int x, int y, int lrow, int hrow, int lcol, int hcol) {
  int i;
  tgt->x = x;
  for(i = lcol; i < hcol; i++) {
    tgt->y = y;
    copy_column(tiles, tgt, lvl, lrow, hrow, i);
    tgt->x += 16;
  }
}

int main(int argc, char *argv[]) {
  TOMREGS->vmode = RGB16|CSYNC|BGEN|PWIDTH4|VIDEN;
  init_interrupts();
  init_display_driver();

  display *d = new_display(0);

  d->x = 20;
  d->y = 16;

  memcpy((void*)TOMREGS->clut1, egyptPal, NCOLS*sizeof(uint16_t));

#ifdef DEBUG
  FILE *console = open_console(d, 0, HEIGHT, NCOLS / 2 + 1);
#endif

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

  sprite *black_border1 = new_sprite(16, 1, -16 + 8, 0, DEPTH16, &black);
  black_border1->trans = 0;
  black_border1->dwidth = 0;
  black_border1->height = HEIGHT;

  sprite *black_border2 = new_sprite(16, 1, WIDTH - 8, 0, DEPTH16, &black);
  black_border2->trans = 0;
  black_border2->dwidth = 0;
  black_border2->height = HEIGHT;

  clear_screen(screen1);
  clear_screen(screen2);

  screen *scr1 = screen1;
  screen *scr2 = screen2;
  sprite *spr1 = screen1_sprite;
  sprite *spr2 = screen2_sprite;

  attach_sprite_to_display_at_layer(screen1_sprite, d, 0);
  attach_sprite_to_display_at_layer(screen2_sprite, d, 0);
  attach_sprite_to_display_at_layer(black_border1, d, 1);
  attach_sprite_to_display_at_layer(black_border2, d, 1);

  init_map();
  copy_zone(tiles, scr1, &egyptLevel, 0, 0, 0, HEIGHT/16, 0, WIDTH/16);
  copy_zone(tiles, scr2, &egyptLevel, 0, 0, 0, HEIGHT/16, WIDTH/16 + 1, WIDTH/16 + 2);

  show_display(d);

#ifdef DEBUG
  fprintf(console, "nrows = %d, ncols = %d\n", egyptLevel.nrows, egyptLevel.ncols);
  fprintf(console, "width = %d, height = %d\n", video_width / 4, video_height);
#endif

  joypad_state joypads;

  while(1) {
    vsync();
    read_joypad_state(&joypads);
    unsigned long cmd = joypads.j1;

    if(cmd & JOYPAD_RIGHT) {
      if (map_x < map_w - WIDTH) {
        map_x++;

        if (spr1->x <= -WIDTH) {
          spr1->x += 2 * WIDTH;

          screen *tmp_scr = scr1;
          scr1 = scr2;
          scr2 = tmp_scr;

          sprite *tmp_spr = spr1;
          spr1 = spr2;
          spr2 = tmp_spr;
        }

        spr1->x--;
        spr2->x--;

        if ((map_x & 15) == 8) {
          int map_tx = map_x >> 4;
          int dx = (map_tx << 4) % WIDTH;
          copy_zone(tiles, scr2, &egyptLevel, dx, 0, 0, HEIGHT/16, WIDTH/16 + map_tx, WIDTH/16 + map_tx + 1);
        }
      }
    } else if(cmd & JOYPAD_LEFT) {
      if (map_x > 0) {
        map_x--;

        if (spr2->x >= WIDTH) {
          spr2->x -= 2 * WIDTH;

          screen *tmp_scr = scr1;
          scr1 = scr2;
          scr2 = tmp_scr;

          sprite *tmp_spr = spr1;
          spr1 = spr2;
          spr2 = tmp_spr;
        }

        spr1->x++;
        spr2->x++;

        if ((map_x & 15) == 8) {
          int map_tx = map_x >> 4;
          int dx = (map_tx << 4) % WIDTH;
          copy_zone(tiles, scr1, &egyptLevel, dx, 0, 0, HEIGHT/16, map_tx, map_tx + 1);
        }
      }
    }

    wait_display_refresh();
  }

  free(screen1_sprite);
  free(screen1_data);
  free(screen1);
  free(screen2_sprite);
  free(screen2_data);
  free(screen2);
  free(tiles);
  free(tile_data);
}

