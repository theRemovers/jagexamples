#include <jagdefs.h>
#include <jagtypes.h>
#include <stdlib.h>

#include <interrupt.h>
#include <display.h>
#include <sprite.h>

#include <skunk.h>
#include <console.h>

int main(int argc, char *argv[]) {
  TOMREGS->vmode = RGB16|CSYNC|BGEN|PWIDTH4|VIDEN;
  init_interrupts();
  init_display_driver();

  display *d = new_display(0);
  d->x = 32;
  d->y = 8;

  FILE *console = open_console(d,8,16,0);

  show_display(d);

  fprintf(console,"Welcome to Example 16 of\nThe Removers Library\n");
  fprintf(console,"This example demonstrates communication with rmvjcp\n");
  fprintf(console,"Do not forget to add -c option when launching rmvjcp\n");

  skunk_init();

  FILE *skerr = skunk_stderr();
  FILE *skin = skunk_stdin();

  fprintf(skerr,"Hello world!\n");

  char buffer[16];

  while(1) {
    char *r = fgets(buffer, sizeof(buffer), skin);
    buffer[15] = 0;
    if(r == NULL) {
      break;
    }
    fprintf(console,"You typed: %s\n",buffer);
    fprintf(skerr,"You typed: %s\n",buffer);
  }

  fprintf(skerr,"Bye bye.");

  fclose(skerr);
  fclose(skin);

  free(skin);
  free(skerr);

  fprintf(console,"Hope you enjoyed it.");

  for(;;) {
    vsync();
  }

  return 0;
}

