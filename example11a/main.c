#include <jagdefs.h>
#include <jagtypes.h>
#include <stdlib.h>
#include <stdio.h>

#include <interrupt.h>
#include <sprite.h>
#include <joypad.h>
#include <sound.h>
#include <screen.h>
#include <blit.h>
#include <fb2d.h>
#include <render.h>
#include <console.h>

#include "data.h"

#define DEBUG 0

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

#define ZOOM 5
#define SCALE 5<<5

#define V_A {(5*ZOOM)<<16, (1*ZOOM)<<16, 40<<16, 10<<16, 0, 0}
#define V_B {(18*ZOOM)<<16, (10*ZOOM)<<16, 80<<16, 20<<16, 0<<16, 0<<16}
#define V_C {(3*ZOOM)<<16, (35*ZOOM)<<16, 120<<16, 30<<16, 0, 0}
#define V_D {(25*ZOOM)<<16, (50*ZOOM)<<16, 50<<16, 40<<16, 63<<16, 0<<16}
#define V_E {(25*ZOOM)<<16, (25*ZOOM)<<16, 60<<16, 50<<16, 63<<16, 63<<16}
#define V_F {(15*ZOOM)<<16, (55*ZOOM)<<16, 70<<16, 60<<16, 0, 0}
#define V_G {(37*ZOOM)<<16, (30*ZOOM)<<16, 80<<16, 55<<16, 0, 0}
#define V_H {(3*ZOOM)<<16, (10*ZOOM)<<16, 140<<16, 50<<16, 0, 0}
#define V1 {0x3a0000, 0xd60000, 0, 10<<16, 0, 0 }
#define V2 {0xae0000, 0xce0000, 0, 10<<16, 0, 0 }
#define V3 {0xac0000, 0xc90000, 0x44880, 15<<16, 0, 0 }
#define V4 {0x370000, 0xd20000, 0xaf6380, 15<<16, 0, 0}

#define NB_VERTICES 12

vertex vertices[NB_VERTICES] = { V_A, V_B, V_C, V_D,
				 V_E, V_F, V_G, V_H,
				 V1, V2, V3, V4};

/* #undef ZBUFFERING */
/* #define ZBUFFERING 0 */

polygon p1 = { NULL, ZBUFFERING|GRDSHADING, 3, RED, NULL, {V_B, V_A, V_C} }; 
polygon p2 = { &p1, ZBUFFERING, 3, GREEN, NULL, {V_C, V_D, V_B} }; 
polygon p3 = { &p2, ZBUFFERING|GRDSHADING|TXTMAPPING, 3, YELLOW, NULL, {V_B, V_D, V_E} };
polygon p4 = { &p3, ZBUFFERING|GRDSHADING, 3, CYAN, NULL, {V_F, V_D, V_C} };
polygon p5 = { &p4, ZBUFFERING|GRDSHADING, 3, BLUE, NULL, {V_D, V_G, V_E} };
polygon p6 = { &p5, ZBUFFERING, 3, MAGENTA, NULL, {V_H, V_F, V_G}};

polygon pa = {NULL, FLTSHADING, 7, MAGENTA, NULL, {V_A, V_C, V_F, V_D, V_G, V_E, V_B}};

polygon ptest = {NULL, ZBUFFERING|GRDSHADING, 4, BLUE, NULL, {V1,V2,V3,V4}};

screen *texture;

void set_polygons() {
  // p1
  p1.vertices[0] = vertices[1];
  p1.vertices[1] = vertices[0];
  p1.vertices[2] = vertices[2];
  // p2 
  p2.vertices[0] = vertices[2];
  p2.vertices[1] = vertices[3];
  p2.vertices[2] = vertices[1];
  // p3
  p3.vertices[0] = vertices[1];
  p3.vertices[1] = vertices[3];
  //  p3.vertices[2] = vertices[6];
  p3.vertices[2] = vertices[4];
  p3.texture = texture;
  // p4
  p4.vertices[0] = vertices[5];
  p4.vertices[1] = vertices[3];
  p4.vertices[2] = vertices[2];
  // p5
  p5.vertices[0] = vertices[3];
  p5.vertices[1] = vertices[6];
  p5.vertices[2] = vertices[4];
  // p6
  p6.vertices[0] = vertices[7];
  p6.vertices[1] = vertices[5];
  p6.vertices[2] = vertices[6];
  // pa
  pa.vertices[0] = vertices[0];
  pa.vertices[1] = vertices[2];
  pa.vertices[2] = vertices[5];
  pa.vertices[3] = vertices[3];
  pa.vertices[4] = vertices[6];
  pa.vertices[5] = vertices[4];
  pa.vertices[6] = vertices[1];
  pa.texture = texture;
  //
  ptest.vertices[0] = vertices[8];
  ptest.vertices[1] = vertices[9];
  ptest.vertices[2] = vertices[10];
  ptest.vertices[3] = vertices[11];
}

void show_polygon(FILE *console, polygon *p) {
  int i;
  int n = p->size;
  fprintf(console,"%d vertices\n",n);
  for(i = 0; i < n; i++) {
    fprintf(console,"X = %08x\n",p->vertices[i].x);
    fprintf(console,"Y = %08x\n",p->vertices[i].y);
    fprintf(console,"I = %08x\n",p->vertices[i].i);
    fprintf(console,"Z = %08x\n",p->vertices[i].z);
    fprintf(console,"U = %08x\n",p->vertices[i].u);
    fprintf(console,"V = %08x\n",p->vertices[i].v);
  }
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

  FILE *console = open_console(d,0,0,0);

  fprintf(console,"Hello\n"); 
  fprintf(console,"Press B to see some polygons\n"); 
  fprintf(console,"Beware: no clipping!!\n"); 
  fprintf(console,"A/C: change active vertex\n");
  fprintf(console,"Left/Right/Up/Down: move active vertex\n");
  fprintf(console,"B+Up/Down: change intensity of active vertex\n");
  fprintf(console,"B+Left/Right: change depth of active vertex\n");
  fprintf(console,"*/#: change intensity of texture\n");

#if(DEBUG)

  fprintf(console,"Press B to clear the buffers\n");
  fprintf(console,"BASE ADDRESS = %08x = %08x\n",frame1->data,frame_data);
  unsigned long flags = *((unsigned long *)frame1);
  fprintf(console,"FLAGS = %b = %08x\n",flags,flags);
  fprintf(console,"HEIGHT = %d\n",frame1->height);  
  fprintf(console,"WIDTH = %d\n",frame1->width);  
 
  for(;;) {
    vsync();
    read_joypad_state(j_state);
    if(j_state->j1 & JOYPAD_B) {
      break;
    }
  }

  for(;;) {
    vsync();
    read_joypad_state(j_state);
    if(!(j_state->j1 & JOYPAD_B)) {
      break;
    }
  }

  int phase = 0;

  for(phase = 0; phase < 3; phase++) {

    switch(phase) {
    case 0:
      fprintf(console,"1-simple clear with 68k...\n");
      bzero(frame1->data,320*240*2);
      break;
    case 1:
      fprintf(console,"2-simple clear with blitter...\n");
      //      asm("illegal");      
      clear_screen(frame1);
      break;
    case 2:
      fprintf(console,"3-Zbuffer clear with blitter...\n");
      //      asm("illegal");      
      clear_zbuffered_screen(frame1);
      break;
    }

    fprintf(console,"Press B to continue\n");
    
    for(;;) {
      vsync();
      read_joypad_state(j_state);
      if(j_state->j1 & JOYPAD_B) {
	break;
      }
    }

    for(;;) {
      vsync();
      read_joypad_state(j_state);
      if(!(j_state->j1 & JOYPAD_B)) {
	break;
      }
    }
  }

#else
  fprintf(console,"Press B to continue\n");
  
  for(;;) {
    vsync();
    read_joypad_state(j_state);
    if(j_state->j1 & JOYPAD_B) {
      break;
    }
  }
  
  for(;;) {
    vsync();
    read_joypad_state(j_state);
    if(!(j_state->j1 & JOYPAD_B)) {
      break;
    }
  }
#endif

  fclose(console);

  attach_sprite_to_display_at_layer(frame_sprite,d,1);

  screen *phys = frame1;
  screen *log = frame2;
  screen *tmp = phys;

  clear_zbuffered_screen(frame1);

  int idx = 0;
  unsigned long joy = 0;
  for(;;) {
    vsync();
    read_joypad_state(j_state);
    set_polygons();

    SET_BG(MAGENTA);
    //    clear_zbuffered_screen(phys);
    SET_BG(RED);
    //    p3.next = NULL;
    render_polygon_list_and_wait(phys,&p6,CLR_Z_SCREEN);
    //    render_polygon_list_and_wait(phys,&ptest);
    SET_BG(0);
    vertex *v = &vertices[idx];
    unsigned long o_joy = joy;
    joy = j_state->j1;
    unsigned long n_joy = joy;
    n_joy &= ~o_joy; 
    if(n_joy & JOYPAD_C) {
      idx++;
      if(idx >= NB_VERTICES) {
	idx = 0;
      }
    } else if(n_joy & JOYPAD_A) {
      idx--;
      if(idx < 0) {
	idx = NB_VERTICES-1;
      }
    }
    if(j_state->j1 & JOYPAD_B) {
      if(j_state->j1 & JOYPAD_DOWN) {
	v->i += 0x10000;
      } else if(j_state->j1 & JOYPAD_UP) {
	v->i -= 0x10000;
      }
      if(j_state->j1 & JOYPAD_LEFT) {
	v->z -= 0x8000;
      } else if(j_state->j1 & JOYPAD_RIGHT) {
	v->z += 0x8000;
      }

    } else {
      if(j_state->j1 & JOYPAD_DOWN) {
	v->y += 0x8000;
      } else if(j_state->j1 & JOYPAD_UP) {
	v->y -= 0x8000;
      }
      if(j_state->j1 & JOYPAD_LEFT) {
	v->x -= 0x8000;
      } else if(j_state->j1 & JOYPAD_RIGHT) {
	v->x += 0x8000;
      }
    }
    if(j_state->j1 & JOYPAD_STAR) {
      p3.param -= 0x8000;
    } else if(j_state->j1 & JOYPAD_SHARP) {
      p3.param += 0x8000;
    }

    tmp = phys;
    phys = log;
    log = tmp;
    wait_display_refresh();
    frame_sprite->data = phys->data;
  }    

  return 0;
}

