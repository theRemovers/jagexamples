#ifndef __MY_DISPLAY_H
#define __MY_DISPLAY_H

#include <sprite.h>

#define MAX_SPRITES 16

void display_init();

void display_update();
void display_update_sprite(int nb);

void display_refresh();

void display_cleanup();

extern sprite *display_sprites[MAX_SPRITES];

extern int16_t display_offset_x;
extern int16_t display_offset_y;
extern uint16_t display_interlaced;

extern uint16_t VI;

#endif
