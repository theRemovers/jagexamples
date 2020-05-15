#include <jagdefs.h>
#include <jagtypes.h>
#include <stdlib.h>
#include <stdio.h>

#include <interrupt.h>
#include <sprite.h>
#include <joypad.h>
#include <screen.h>
#include <blit.h>
#include <fb2d.h>
#include <render.h>
#include <console.h>
#include <sound.h>

#include "data.h"

#define SIDESHADING GRDSHADING
//#define SIDESHADING FLTSHADING

#undef SET_BG 

#define RED 0xf0ff
#define GREEN 0x7fff
#define BLUE 0x00ff
#define CYAN 0x0fff
#define MAGENTA 0x70ff
#define YELLOW 0xffff
#define WHITE 0x88ff
#define GRAY 0x88c0

#ifndef SET_BG
#define SET_BG(x) {}
#endif

/* #undef GRDSHADING */
/* #undef TXTMAPPING  */

#ifndef GRDSHADING
#define GRDSHADING 0
#endif

#ifndef TXTMAPPING
#define TXTMAPPING 0
#endif

#define V_TEMPL {0, 0, 0, 0, 0, 0}

polygon petiq = {NULL, GRDSHADING|TXTMAPPING, 4, BLUE, NULL, {V_TEMPL, V_TEMPL, V_TEMPL, V_TEMPL}};

polygon pmetal = {&petiq, GRDSHADING, 4, WHITE, NULL, {V_TEMPL, V_TEMPL, V_TEMPL, V_TEMPL}};
polygon pavant = {&pmetal, GRDSHADING, 4, BLUE, NULL, {V_TEMPL, V_TEMPL, V_TEMPL, V_TEMPL}};
polygon parriere = {&pavant, GRDSHADING, 4, BLUE, NULL, {V_TEMPL, V_TEMPL, V_TEMPL, V_TEMPL}};

polygon pside1 = {&parriere, SIDESHADING, 4, BLUE, NULL, {V_TEMPL, V_TEMPL, V_TEMPL, V_TEMPL}};
polygon pside2 = {&pside1, SIDESHADING, 4, BLUE, NULL, {V_TEMPL, V_TEMPL, V_TEMPL, V_TEMPL}};
polygon pside3 = {&pside2, SIDESHADING, 4, BLUE, NULL, {V_TEMPL, V_TEMPL, V_TEMPL, V_TEMPL}};
polygon pside4 = {&pside3, SIDESHADING, 4, BLUE, NULL, {V_TEMPL, V_TEMPL, V_TEMPL, V_TEMPL}};

screen *texture;

#define XCoord(idx) (X2D[idx]<<16)
#define YCoord(idx) (Y2D[idx]<<16)

int light = 80<<16;

#define SAT24(x) (x < 0 ? 0 : (x > 0xffffff ? 0xffffff : x))
#define I(idx) (SAT24(light + (Z3D[19+idx]<<7)))
//#define I(idx) ((Z3D[19+idx]<<7))

void switch_flat_mode(int flag) {
  if(flag) {
    petiq.flags = FLTMAPPING|TXTMAPPING;
  } else {
    petiq.flags = TXTMAPPING;
  }
  pmetal.flags = FLTSHADING;
  pavant.flags = FLTSHADING;
  parriere.flags = FLTSHADING;
  pside1.flags = FLTSHADING;
  pside2.flags = FLTSHADING;
  pside3.flags = FLTSHADING;
  pside4.flags = FLTSHADING;
}

void switch_gouraud_mode() {
  petiq.flags = GRDSHADING|TXTMAPPING;
  pmetal.flags = GRDSHADING;
  pavant.flags = GRDSHADING;
  parriere.flags = GRDSHADING;
  pside1.flags = GRDSHADING;
  pside2.flags = GRDSHADING;
  pside3.flags = GRDSHADING;
  pside4.flags = GRDSHADING;
}

void set_object() {
  // face avant
  pavant.vertices[0].x = XCoord(0);
  pavant.vertices[0].y = YCoord(0);
  pavant.vertices[1].x = XCoord(1);
  pavant.vertices[1].y = YCoord(1);
  pavant.vertices[2].x = XCoord(2);
  pavant.vertices[2].y = YCoord(2);
  pavant.vertices[3].x = XCoord(3);
  pavant.vertices[3].y = YCoord(3);

  pavant.vertices[0].i = I(0);
  pavant.vertices[1].i = I(1);
  pavant.vertices[2].i = I(2);
  pavant.vertices[3].i = I(3);

  pavant.param &= 0xff00;
  pavant.param |= (((I(0)+I(1)+I(2)+I(3))>>2) >> 16)  & 0xff;
  // etiquette
  petiq.vertices[0].x = XCoord(4);
  petiq.vertices[0].y = YCoord(4);
  petiq.vertices[1].x = XCoord(5);
  petiq.vertices[1].y = YCoord(5);
  petiq.vertices[2].x = XCoord(6);
  petiq.vertices[2].y = YCoord(6);
  petiq.vertices[3].x = XCoord(7);
  petiq.vertices[3].y = YCoord(7);

  petiq.vertices[0].i = I(4)-(50<<16);
  petiq.vertices[1].i = I(5)-(50<<16);
  petiq.vertices[2].i = I(6)-(50<<16);
  petiq.vertices[3].i = I(7)-(50<<16);

  if(petiq.flags & GRDSHADING) {
    texture->data = &texture_gfx;
  } else {
    texture->data = &texture2_gfx;
  }
  petiq.texture = texture;
  petiq.vertices[0].u = 1<<16;
  petiq.vertices[0].v = 1<<16;
  petiq.vertices[1].u = (TEXTURE_WIDTH-1)<<16;
  petiq.vertices[1].v = 1<<16;
  petiq.vertices[2].u = (TEXTURE_WIDTH-1)<<16;
  petiq.vertices[2].v = (TEXTURE_HEIGHT-1)<<16;
  petiq.vertices[3].u = 1<<16;
  petiq.vertices[3].v = (TEXTURE_HEIGHT-1)<<16;

  petiq.param &= 0xff00;
  petiq.param |= (((I(4)+I(5)+I(6)+I(7))>>2) >> 16)  & 0xff;

  petiq.param = ((I(4)+I(5)+I(6)+I(7))>>2) ^ 0x800000;
  // partie metallique
  pmetal.vertices[0].x = XCoord(8);
  pmetal.vertices[0].y = YCoord(8);
  pmetal.vertices[1].x = XCoord(9);
  pmetal.vertices[1].y = YCoord(9);
  pmetal.vertices[2].x = XCoord(10);
  pmetal.vertices[2].y = YCoord(10);
  pmetal.vertices[3].x = XCoord(11);
  pmetal.vertices[3].y = YCoord(11);

  pmetal.vertices[0].i = I(8);
  pmetal.vertices[1].i = I(9);
  pmetal.vertices[2].i = I(10);
  pmetal.vertices[3].i = I(11);

  pmetal.param &= 0xff00;
  pmetal.param |= (((I(8)+I(9)+I(10)+I(11))>>2) >> 16)  & 0xff;
  // face arriere
  parriere.vertices[0].x = XCoord(15);
  parriere.vertices[0].y = YCoord(15);
  parriere.vertices[1].x = XCoord(18);
  parriere.vertices[1].y = YCoord(18);
  parriere.vertices[2].x = XCoord(17);
  parriere.vertices[2].y = YCoord(17);
  parriere.vertices[3].x = XCoord(16);
  parriere.vertices[3].y = YCoord(16);

  parriere.vertices[0].i = I(15);
  parriere.vertices[1].i = I(18);
  parriere.vertices[2].i = I(17);
  parriere.vertices[3].i = I(16);

  parriere.param &= 0xff00;
  parriere.param |= (((I(15)+I(18)+I(17)+I(16))>>2) >> 16)  & 0xff;
  //
  pside1.vertices[0].x = XCoord(15);
  pside1.vertices[0].y = YCoord(15);
  pside1.vertices[1].x = XCoord(16);
  pside1.vertices[1].y = YCoord(16);
  pside1.vertices[2].x = XCoord(1);
  pside1.vertices[2].y = YCoord(1);
  pside1.vertices[3].x = XCoord(0);
  pside1.vertices[3].y = YCoord(0);

  pside1.vertices[0].i = I(15);
  pside1.vertices[1].i = I(16);
  pside1.vertices[2].i = I(1);
  pside1.vertices[3].i = I(0);

  pside1.param &= 0xff00;
  pside1.param |= (((I(15)+I(16)+I(1)+I(0))>>2) >> 16)  & 0xff;
  //
  pside2.vertices[0].x = XCoord(17);
  pside2.vertices[0].y = YCoord(17);
  pside2.vertices[1].x = XCoord(18);
  pside2.vertices[1].y = YCoord(18);
  pside2.vertices[2].x = XCoord(3);
  pside2.vertices[2].y = YCoord(3);
  pside2.vertices[3].x = XCoord(2);
  pside2.vertices[3].y = YCoord(2);

  pside2.vertices[0].i = I(17);
  pside2.vertices[1].i = I(18);
  pside2.vertices[2].i = I(3);
  pside2.vertices[3].i = I(2);

  pside2.param &= 0xff00;
  pside2.param |= (((I(17)+I(18)+I(3)+I(2))>>2) >> 16)  & 0xff;
  // 
  pside3.vertices[0].x = XCoord(16);
  pside3.vertices[0].y = YCoord(16);
  pside3.vertices[1].x = XCoord(17);
  pside3.vertices[1].y = YCoord(17);
  pside3.vertices[2].x = XCoord(2);
  pside3.vertices[2].y = YCoord(2);
  pside3.vertices[3].x = XCoord(1);
  pside3.vertices[3].y = YCoord(1);

  pside3.vertices[0].i = I(16);
  pside3.vertices[1].i = I(17);
  pside3.vertices[2].i = I(2);
  pside3.vertices[3].i = I(1);

  pside3.param &= 0xff00;
  pside3.param |= (((I(16)+I(17)+I(2)+I(1))>>2) >> 16)  & 0xff;
  //
  pside4.vertices[0].x = XCoord(18);
  pside4.vertices[0].y = YCoord(18);
  pside4.vertices[1].x = XCoord(15);
  pside4.vertices[1].y = YCoord(15);
  pside4.vertices[2].x = XCoord(0);
  pside4.vertices[2].y = YCoord(0);
  pside4.vertices[3].x = XCoord(3);
  pside4.vertices[3].y = YCoord(3);

  pside4.vertices[0].i = I(18);
  pside4.vertices[1].i = I(15);
  pside4.vertices[2].i = I(0);
  pside4.vertices[3].i = I(3);

  pside4.param &= 0xff00;
  pside4.param |= (((I(18)+I(15)+I(0)+I(3))>>2) >> 16)  & 0xff;
}

void draw_object(screen *dst) {
  set_object();
  render_polygon_list(dst,&pside4,CLR_SCREEN);
}

int main(int argc, char *argv[]) {
  joypad_state *j_state = malloc(sizeof(joypad_state));

  TOMREGS->vmode = CRY16|CSYNC|BGEN|PWIDTH4|VIDEN;
  init_interrupts();

  init_display_driver();

  display *d = new_display(0);

  d->x = 32;
  d->y = 16;

  texture = new_screen();
  set_simple_screen(DEPTH16,TEXTURE_WIDTH,TEXTURE_HEIGHT,texture,&texture_gfx);

  sprite *texture_sprite = sprite_of_screen(0,0,texture);

/*   texture2 = new_screen(); */
/*   set_simple_screen(DEPTH16,256,192,texture2,&texture2_gfx); */

  screen *frame1 = new_screen();
  screen *frame2 = new_screen();
  phrase *frame_data = alloc_z_double_buffered_screens(DEPTH16,320,240,frame1,frame2);
  sprite *frame_sprite = sprite_of_screen(0,0,frame1);

  frame_sprite->trans = 1;

  void *renderer_addr = &_GPU_FREE_RAM;
  void *gpu_available_ram = init_renderer(renderer_addr);

  //  attach_sprite_to_display_at_layer(texture_sprite,d,15);

  show_display(d);

  attach_sprite_to_display_at_layer(frame_sprite,d,1);

  screen *phys = frame1;
  screen *log = frame2;
  screen *tmp = phys;

  clear_screen(frame1);

  init_3D();
  for(;;) {
    vsync();
    SET_BG(RED);
    //    clear_screen(phys);
    SET_BG(BLUE);
    draw_object(phys);
    animate_3D();
    read_joypad_state(j_state);
    SET_BG(YELLOW);
    wait_renderer_completion();
    SET_BG(0);
    tmp = phys;
    phys = log;
    log = tmp;
    wait_display_refresh();
    frame_sprite->data = phys->data;

    if(j_state->j1 & JOYPAD_SHARP) {
      light += 0x10000;
    } else if(j_state->j1 & JOYPAD_STAR) {
      light -= 0x10000;
    }
    if(j_state->j1 & JOYPAD_X) {
      switch_flat_mode(0);
    } else if(j_state->j1 & JOYPAD_Y) {
      switch_flat_mode(1);
    } else if(j_state->j1 & JOYPAD_Z) {
      switch_gouraud_mode();
    }
  }

  return 0;
}

