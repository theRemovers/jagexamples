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
  uint16_t nrows;
  uint16_t ncols;
  uint8_t data[];
} level_t;

extern level_t egyptLevel;

#define WIDTH 320
#define HEIGHT 192

#include <console.h>

#define SWAP(typ, v1, v2) { typ tmp = v1; v1 = v2; v2 = tmp; }

int main(int argc, char *argv[]) {
  TOMREGS->vmode = RGB16|CSYNC|BGEN|PWIDTH4|VIDEN;
  init_interrupts();
  init_display_driver();

  display *d = new_display(0);

  memcpy((void*)TOMREGS->clut1, egyptPal, NCOLS*sizeof(uint16_t));

  FILE *console = open_console(d, 0, HEIGHT, NCOLS / 2 + 1);

  screen *tiles = new_screen();
  set_simple_screen(DEPTH8, WIDTH, HEIGHT, tiles, (phrase *)egyptTiles);

  screen *bg_screen_log = new_screen();
  screen *bg_screen_phys = new_screen();
  phrase *bg_gfx = alloc_double_buffered_screens(DEPTH8, WIDTH, HEIGHT, bg_screen_log, bg_screen_phys);

  sprite *bg_sprite_log = sprite_of_screen(0, 0, bg_screen_log);
  sprite *bg_sprite_phys = sprite_of_screen(0, 0, bg_screen_phys);
  bg_sprite_log->trans = 0;
  bg_sprite_log->invisible = 1;
  bg_sprite_phys->trans = 0;
  bg_sprite_phys->invisible = 0;
  clear_screen(bg_screen_phys);
  clear_screen(bg_screen_log);

  attach_sprite_to_display_at_layer(bg_sprite_phys, d, 1);
  attach_sprite_to_display_at_layer(bg_sprite_log, d, 1);
  
  screen_copy_straight(tiles, bg_screen_phys, WIDTH, HEIGHT, MODE_S);
  
  show_display(d);

  fprintf(console, "nrows = %d, ncols = %d\n", egyptLevel.nrows, egyptLevel.ncols);

  while(1) {
    vsync();
    bg_screen_phys->x = 1;
    bg_screen_phys->y = 0;
    bg_screen_log->x = 0;
    bg_screen_log->y = 0;
    screen_copy_straight(bg_screen_phys, bg_screen_log, WIDTH-1, HEIGHT, MODE_S);
    wait_display_refresh();
    SWAP(sprite *, bg_sprite_log, bg_sprite_phys);
    SWAP(screen *, bg_screen_log, bg_screen_phys);
    bg_sprite_log->invisible = 1;
    bg_sprite_phys->invisible = 0;
  }
}
