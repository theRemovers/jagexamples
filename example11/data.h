#ifndef DATA_H
#define DATA_H

#include <jagtypes.h>

#define TEXTURE_WIDTH 96
#define TEXTURE_HEIGHT 72
/* #define TEXTURE_WIDTH 128 */
/* #define TEXTURE_HEIGHT 64 */
/* #define TEXTURE_WIDTH 256 */
/* #define TEXTURE_HEIGHT 64 */
extern phrase texture_gfx;
extern phrase texture2_gfx;

extern void init_3D();

extern void animate_3D();

extern short int X2D[];
extern short int Y2D[];
extern int Z3D[];

extern short int A;
extern short int B;
extern short int C;

extern short int pause;

#endif
