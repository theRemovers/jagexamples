#include <jagdefs.h>
#include <jagtypes.h>
#include <stdlib.h>

#include <interrupt.h>
#include <display.h>
#include <sprite.h>

#include <joypad.h>

#include <console.h>

extern animation_chunk imp_animation;
extern phrase imp1_gfx;

int main(int argc, char *argv[]) {
  TOMREGS->vmode = RGB16|CSYNC|BGEN|PWIDTH4|VIDEN;
  init_interrupts();
  init_display_driver();

  display *d = new_display(0);
  d->x = 16;
  d->y = 8;
  FILE *console = open_console(d,8,16,0);

  show_display(d);

  fprintf(console,"Welcome to Example 5 of\nThe Removers Library v 1.1.1\n");

  fprintf(console,"Press [UP] or [DOWN] to animate imp #2\n");

  joypad_state *j_state = malloc(sizeof(joypad_state));

  sprite *s1 = new_sprite(60,60,10,10,DEPTH16,&imp1_gfx);
  s1->animation = &imp_animation;
  s1->animated = 1;
  s1->animation_data.counter = 1;
  s1->animation_data.index = 0;
  int loop_counter = 0;

  sprite *s2 = new_sprite(60,60,100,10,DEPTH16,&imp1_gfx);
  int anim_idx = 0;

  attach_sprite_to_display_at_layer(s1,d,1);
  attach_sprite_to_display_at_layer(s2,d,1);

  long old_joy = 0;
  long joy = 0;

  while(1) {
    vsync();
    read_joypad_state(j_state);

    joy = j_state->j1;
    joy &= ~old_joy;
    old_joy = j_state->j1;

    if(joy & JOYPAD_DOWN) {
      anim_idx++;
      if(anim_idx > 6) {
	anim_idx = 0;
      }
      s2->data = (&imp_animation)[anim_idx].data;
    } else if(joy & JOYPAD_UP) {
      anim_idx--;
      if(anim_idx < 0) {
	anim_idx = 6;
      }
      s2->data = (&imp_animation)[anim_idx].data;
    }

    if(s1->animation_data.has_looped) {
      fprintf(console,"Imp #1 animation has looped %d times\n",++loop_counter);
    }
  }

  return 0;
}

