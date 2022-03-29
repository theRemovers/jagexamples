#include <jagdefs.h>
#include <jagtypes.h>

#include <stdlib.h>

#include <interrupt.h>
#include <display.h>
#include <sprite.h>

#include <console.h>

#define BLOB_W 48
#define BLOB_H 48
#define BLOB_RADIUS (BLOB_W / 2)

#define WIDTH 320
#define HEIGHT 200

#define XMAX (WIDTH - 2 * BLOB_RADIUS)
#define YMAX (HEIGHT - 2 * BLOB_RADIUS)
#define XCENTER ((XMAX / 2) - BLOB_RADIUS)
#define YCENTER ((YMAX / 2) - BLOB_RADIUS)

#define NB_BLOBS 27

sprite *blobs[NB_BLOBS];

int animation_index[NB_BLOBS];
int animation_speed[NB_BLOBS];
int animation_tick[NB_BLOBS];

#define ANIM_LENGTH 24

extern phrase *blob48_animation[ANIM_LENGTH];

int rand5() {
  static int init = 0;
  static unsigned long next = 0;
  static int available = 0;
  if(!init) {
    srand(2713);
    init = 1;
  }
  int result = 0;
  do {
    if(available < 3) {
      next = rand();
      available = 32;
    }
    result = next & 0x7;
    next = next >> 3;
    available -= 3;
  } while(5 <= result);
  return result;
}

void init_blobs(display *d) {
  int i;
  for(i = 0; i < NB_BLOBS; i++) {
    animation_index[i] = 0;
    animation_tick[i] = 0;
    animation_speed[i] = 1 + rand5();
    sprite *s = new_sprite(BLOB_W, BLOB_H, XCENTER, YCENTER, DEPTH16, blob48_animation[animation_index[i]]);
    s->rmw = 1;
    blobs[i] = s;
    attach_sprite_to_display_at_layer(s,d,0);
  }
}

void do_blobs() {
  int i;
  for(i = 0; i < NB_BLOBS; i++) {
    sprite *s = blobs[i];
    int dx = rand5();
    int dy = rand5();
    int x = s->x + dx - 2;
    int y = s->y + dy - 2;
    if((x <= 0) || (XMAX <= x)) {
      x = XCENTER;
    }
    if((y <= 0) || (YMAX <= y)) {
      y = YCENTER;
    }
    s->x = x;
    s->y = y;
    animation_tick[i]++;
    if(animation_tick[i] >= animation_speed[i]) {
      animation_index[i]++;
      if(animation_index[i] >= ANIM_LENGTH) {
	animation_index[i] = 0;
      }
      animation_tick[i] = 0;
      s->data = blob48_animation[animation_index[i]];
    }
  }
}

int main(int argc, char *argv[]) {
  TOMREGS->vmode = CRY16|CSYNC|BGEN|PWIDTH4|VIDEN;
  init_interrupts();
  init_display_driver();

  TOMREGS->bg = 0x8800;
  display *d = new_display(0);

  init_blobs(d);
  show_display(d);

  d->x = 16;

  while(1) {
    vsync();
    do_blobs();
  }
}
