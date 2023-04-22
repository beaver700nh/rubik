#ifndef RUBIK_H
#define RUBIK_H

#include <curses.h>

#include "calc.h"

#define RUBIK_DIMS 3
#define CUBIE_SIZE_X0 4
#define CUBIE_SIZE_X1 6
#define CUBIE_SIZE_Y0 2
#define CUBIE_SIZE_Y1 3

typedef struct {
  short color;
  short sign;
} facie_t;

void facie_init(facie_t *facie, short color, short sign);

typedef struct {
  facie_t facies[RUBIK_DIMS];
} cubie_t;

void cubie_init(cubie_t *cubie, unsigned short size, vec3_t pos);

typedef struct {
  unsigned short size;
  cubie_t *cubies; // 3D array [x][y][z]
} rubik_t;

void rubik_init(rubik_t *rubik, unsigned short size);
cubie_t *rubik_get(rubik_t *rubik, vec3_t position);
void rubik_draw(rubik_t *rubik, WINDOW *window);
void rubik_draw_face_xy0(rubik_t *rubik, WINDOW *window, unsigned short row, unsigned short col);
void rubik_dump(rubik_t *rubik, WINDOW *window, unsigned short position);
rubik_t rubik_rotated(rubik_t *rubik, rubik_rotator_t rotator);
void rubik_free(rubik_t *rubik);

#endif
