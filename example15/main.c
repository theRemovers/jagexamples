#include <jagdefs.h>
#include <jagtypes.h>
#include <stdlib.h>

#include <interrupt.h>
#include <display.h>
#include <sprite.h>

#include <sound.h>

#include <joypad.h>

#include <console.h>

extern char ocean_snd;
#define OCEAN_LEN 167928

extern char muzaxx;

#define RED 0xf800
#define BLUE 0x07c0
#define GREEN 0x003f

void vbl_handler() {
  /* we display in Red the time consumed by the 68k protracker routine */
#ifdef DISPLAY_BG
  TOMREGS->bg = RED;
#endif
  play_module();
#ifdef DISPLAY_BG
  TOMREGS->bg = 0; 
#endif
}

int main(int argc, char *argv[]) {
  joypad_state *j_state = malloc(sizeof(joypad_state));
  TOMREGS->vmode = RGB16|CSYNC|BGEN|PWIDTH4|VIDEN;
  init_interrupts();
  init_display_driver();

#ifdef PAULA
  int replay_freq = 50000;
#else
  int replay_freq = 16000;
#endif
  replay_freq = init_sound_driver(replay_freq);

  display *d = new_display(0);
  d->x = 32;
  d->y = 8;
  FILE *console = open_console(d,8,16,0);

  show_display(d);

  fprintf(console,"Welcome to Example 15 of\nThe Removers Library v 1.3.2\n");
  fprintf(console,"This example demonstrates:\n");
  fprintf(console,"1-the console\n");
  fprintf(console,"2-the sound driver\n\n");
  fprintf(console,"The replay freq is %d\n",replay_freq);

  fprintf(console,"\nWe first replay a 16 bit sample.\nYou can change the volume and the panning of the voice with the cross of joypad #0\n");
  fprintf(console,"Press * when you are fed up with that!\n");

  int panning = 8;
  int volume = 64;
  set_voice(0,VOICE_16|VOICE_BALANCE(panning)|VOICE_VOLUME(volume)|VOICE_FREQ(replay_freq,16000),&ocean_snd,OCEAN_LEN,&ocean_snd,OCEAN_LEN);

  long old_joy = 0;
  long joy = 0;

  int panflag = 0;
  int volflag = 0;

  while(1) {
    vsync();
    read_joypad_state(j_state);

    joy = j_state->j1;
    joy &= ~old_joy;
    old_joy = j_state->j1;

    if(joy & JOYPAD_STAR) {
      break;
    }

    if(joy & JOYPAD_LEFT) {
      if(panning > 0) {
	--panning;
	panflag = 1;
      }
    } else if(joy & JOYPAD_RIGHT) {
      if(panning < 16) {
	++panning;
	panflag = 1;
      }
    }
    if(joy & JOYPAD_UP) {
      if(volume < 64) {
	++volume;
	volflag = 1;
      }
    } else if(joy & JOYPAD_DOWN) {
      if(volume > 0) {
	--volume;
	volflag = 1;
      }
    }
    if(panflag) {
      panflag = 0;
      set_panning(0,panning);
      fprintf(console,"Voice 0 balance set to %d\n",panning);
    }
    if(volflag) {
      volflag = 0;
      set_volume(0,volume);
      fprintf(console,"Voice 0 volume set to %d\n",volume);
    }
  }

  clear_voice(0);

  fprintf(console,"\nHope you have enjoyed this part!\n\n");

  int tempo = 0;
  int nb_voices = init_module(&muzaxx,tempo);

  int voice_mask = (1 << nb_voices) - 1;

  vblQueue[0] = vbl_handler;

  fprintf(console,"We now demonstrate the Protracker replay routine with");
  fprintf(console," a %d voices module\n",nb_voices);
  fprintf(console,"The module is named Checkered Subgliep\n");
  fprintf(console,"and was composed by SCY/tSCc\n");

  fprintf(console,"It is well possible that under an\nemulator the sound is distorted...\nSorry your computer is not fast enough!!\n");

  fprintf(console,"With key 1 to 8 of joypad #0, you can\nenable/disable the corresponding voice\n");
  fprintf(console,"You can also pause/re-play the music\nwith the key pause\n");

  int pause_flag = 0;
  while(1) {
    vsync();
    read_joypad_state(j_state);

    joy = j_state->j1;
    joy &= ~old_joy;
    old_joy = j_state->j1;
    
    if((old_joy & JOYPAD_PAUSE) && !pause_flag) {
      pause_module();
      pause_flag = 1;
    } else if(!(old_joy & JOYPAD_PAUSE) && pause_flag) {
      pause_module();
      pause_flag = 0;
    }
    int mask = voice_mask;
    if(joy & JOYPAD_1) {
      voice_mask ^= 0x1;
    }
    if(joy & JOYPAD_2) {
      voice_mask ^= 0x1<<1;
    }
    if(joy & JOYPAD_3) {
      voice_mask ^= 0x1<<2;
    }
    if(joy & JOYPAD_4) {
      voice_mask ^= 0x1<<3;
    }
    if(joy & JOYPAD_5) {
      voice_mask ^= 0x1<<4;
    }
    if(joy & JOYPAD_6) {
      voice_mask ^= 0x1<<5;
    }
    if(joy & JOYPAD_7) {
      voice_mask ^= 0x1<<6;
    }
    if(joy & JOYPAD_8) {
      voice_mask ^= 0x1<<7;
    }
    if(voice_mask != mask) {
      voice_mask = enable_module_voices(voice_mask);
      fprintf(console,"Voice mask = %08b\n",voice_mask);
    }
    
  }

  return 0;
}

